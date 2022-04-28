#include "mbed.h"//includes mbed library
#include "C12832.h"//includes LCD screen lib
#include <encoder.cpp>
#include <PWMdrive.cpp>
#include <pot.cpp>
#include <sensor.cpp>

#define DEFAULT_SPEED 8.0f//Rad/s

//Desired angular speeds rad/s
double global_ang_speed_right = DEFAULT_SPEED;
double global_ang_speed_left = DEFAULT_SPEED;
bool disable_speed_controls = false;
volatile float Kp;

TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
Potentiometer* leftPot = new SamplingPotentiometer(A0, 3.3, 100.0); 
Potentiometer* rightPot = new SamplingPotentiometer(A1, 3.3, 100.0);
Car car;//Motor control
C12832 lcd(D11, D13, D12, D7, D10);
InterruptIn bluetooth_receive(PA_12);
//Line sensors
SENSOR sen1(PC_4); //grey
SENSOR sen2(PC_0);//blue
SENSOR sen3(PC_5);//green
SENSOR sen4(PC_2);//red
SENSOR sen5(PC_3);//purple
SENSOR sen6(PB_0);//orange (Power drop)

Ticker Speed_tick;
float step = 0.02f;
void Speed_control(void){
        if(disable_speed_controls == false) {      
            if(wheel_left->get_ang_speed() > global_ang_speed_left)car.changeDutyCycle(0.0f, step);
            else if(wheel_left->get_ang_speed() < global_ang_speed_left)car.changeDutyCycle(0.0f, -1.0f*step);
            if(wheel_right->get_ang_speed() > global_ang_speed_right)car.changeDutyCycle(step, 0.0f);
            else if(wheel_right->get_ang_speed() < global_ang_speed_right)car.changeDutyCycle(-1.0f*step, 0.0f);
            //Safeguards so that speed doesn't go to negatives 
            if (global_ang_speed_left < 0.0f)global_ang_speed_left = 0.0f;
            if (global_ang_speed_right < 0.0f)global_ang_speed_right = 0.0f;
        }
}
float pre_error = 0.0;
Ticker control_tick;
void control_PID(void)
{
        int sensorsOn = 0;
        
        float distance = 0.0f;
        Kp = leftPot->getCurrentSampleNorm()*2.0f;//Up to 0.5
        float Kd = 0.0088f;
        
//Checking sensor states
        if (sen1.sensorState() == true){
            distance += 3.0f;//37,3
            sensorsOn += 1;
        }
        if (sen2.sensorState() == true) {
            distance += 2.5f;//23,2.5
            sensorsOn += 1;
        }
        if (sen3.sensorState() == true){
            distance += 1.0f;//10
            sensorsOn += 1;
        }
        if (sen4.sensorState() == true) {
            distance -= 1.0f;//5
            sensorsOn += 1;
        }
        if (sen5.sensorState() == true) {
            distance -= 2.5f;//19
            sensorsOn += 1;
        }
        if (sen6.sensorState() == true){
            distance -= 3.0f;//33
            sensorsOn += 1;
        }
        if(sen3.sensorState() == true && sen4.sensorState() == true){
            distance = 0.0f;
            global_ang_speed_left = DEFAULT_SPEED;
            global_ang_speed_right = DEFAULT_SPEED;
        }
        
        if (sensorsOn > 0) {
            distance = distance/sensorsOn;
            float D__ = (distance - pre_error)/0.01f;
            float D_ = D__ * Kd;
            pre_error = distance;
            disable_speed_controls = false;
            wheel_right->line();
            float P_ = distance*Kp; //creates the p value by a given error(distance) and the controlling variable kp
            float output = P_ + D_; //for future if I_ and D_ are implemented
            if (output > 0) { //line is on the left hand side
                global_ang_speed_left = DEFAULT_SPEED-output;//speeds up right wheel and slows left
                global_ang_speed_right = DEFAULT_SPEED+output;
            } else if (output < 0) { //line is on the right hand side
                global_ang_speed_left = DEFAULT_SPEED-output;//slows right wheel and speeds up left
                global_ang_speed_right = DEFAULT_SPEED+output;
            } else {
//              global_ang_speed_left = DEFAULT_SPEED;
//              global_ang_speed_right = DEFAULT_SPEED;//remains constant forward
            }
        }
        else {//No line
            wheel_right->no_line();
        }
}

//Bluetooth
class BLUETOOTH
{
private:
    Serial hm10; //UART6 TX,RX
    uint8_t i;
    char hm10_receive_buffer[5];
public:
    BLUETOOTH(PinName USBTX, PinName USBRX): hm10(USBTX,USBRX)
    {
        hm10.baud(9600);
    }
    void control()
    {
        if(hm10.readable()) {
            while(i < 5) {
                hm10_receive_buffer[i++] = hm10.getc();
            }
            i = 0;
            if(strcmp(hm10_receive_buffer, "ROUND") == 0) {
                disable_speed_controls = true;
                control_tick.detach();
                global_ang_speed_right = 0.0;
                global_ang_speed_left = 0.0;
                car.setMotorSpeeds(0.8f,0.8f);
                car.turnaround(LEFT);
                memset(hm10_receive_buffer, 0, 5);
            }
            else if(strcmp(hm10_receive_buffer, "PAUSE") == 0){
                control_tick.detach();
                global_ang_speed_right = 0.0;
                global_ang_speed_left = 0.0;
                memset(hm10_receive_buffer, 0, 5);
            }
            else if(strcmp(hm10_receive_buffer, "RESUM") == 0){
                car.setDirectionForward();
                disable_speed_controls = false;
                control_tick.attach(&control_PID, 0.01);
                memset(hm10_receive_buffer, 0, 5);
                }
        }
    }
};//End of Bluetooth
BLUETOOTH BT(PA_11,PA_12);


int main(void)
{
    lcd.set_auto_up(1);//This ensures no flickering on LCD
    Speed_tick.attach(&Speed_control, 0.005);
    control_tick.attach(&control_PID, 0.01);
    bluetooth_receive.rise(&BT, &BLUETOOTH::control);
    while(true){
        lcd.locate(0,0);
        lcd.printf("%.2f /100",Kp*100);
        if(wheel_right->get_flag() == true){//Controlled stop
            global_ang_speed_right = 0.0;
            global_ang_speed_left = 0.0;
        }
    }
}
