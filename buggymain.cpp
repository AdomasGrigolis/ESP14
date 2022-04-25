#include "mbed.h" //includes mbed library
#include "C12832.h" //includes LCD screen lib
#include <encoder.cpp>
#include <PWMdrive.cpp>
#include <pot.cpp>
#include <sensor.cpp>

#define STOPPING_STIFFNESS 0.01f
#define STOPPING_SPEED 0.1f
#define DEFAULT_SPEED 6.0f//Rad/s

//Desired angular speeds rad/s
double global_ang_speed_right = DEFAULT_SPEED;
double global_ang_speed_left = DEFAULT_SPEED;
bool disable_speed_controls = false;

// Max X value of LCD screen
// Max Y value of LCD screen

TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
Potentiometer* leftPot = new SamplingPotentiometer(A0, 3.3, 100.0); 
Potentiometer* rightPot = new SamplingPotentiometer(A1, 3.3, 100.0);
Car car;//Motor control
C12832 lcd(D11, D13, D12, D7, D10);
//Line sensors
SENSOR sen1(PC_4); //grey
SENSOR sen2(PC_0);//blue
SENSOR sen3(PC_5);//green
SENSOR sen4(PC_2);//red
SENSOR sen5(PC_3);//purple
SENSOR sen6(PB_0);//orange (Power drop)

//Deceleration
Ticker decc_tick;
void deceleration()
{
    //Right
    if (wheel_right->get_ang_speed() > 0.0)
    {
        global_ang_speed_right -= STOPPING_STIFFNESS;
    }
    else if (wheel_right->get_ang_speed() < 0.0)
    {
        global_ang_speed_right += STOPPING_STIFFNESS;
    }
    //Left
    if (wheel_left->get_ang_speed() > 0.0)
    {
        global_ang_speed_left -= STOPPING_STIFFNESS;
    }
    else if (wheel_right->get_ang_speed() < 0.0)
    {
        global_ang_speed_right += STOPPING_STIFFNESS;
    }
    if (wheel_right->get_ang_speed() == 0.0 && wheel_left->get_ang_speed() == 0.0)decc_tick.detach();
}

void stopGradual()
{
    decc_tick.detach();
    decc_tick.attach(&deceleration, STOPPING_SPEED);
}//Deceleration end

int main(void)
{
    lcd.set_auto_up(1);//This ensures no flickering on LCD
    int sensorsOn;
    float distance;
    float Kp;
    float weight;
    float step;
    car.setMotorSpeeds(0.7f,0.7f);
    while(1){
        distance = 0.0f;//mm
        sensorsOn = 0;
        Kp = leftPot->getCurrentSampleNorm()/4.0f;//Up to 0.25
        weight = rightPot->getCurrentSampleNorm()*5.0f;//Up to 5
//Checking sensor states
        if (sen1.sensorState() == true){
            distance += 0.0f;//37
            sensorsOn += 0;
        }
        if (sen2.sensorState() == true) {
            distance += weight;//23
            sensorsOn += 1;
        }
        if (sen3.sensorState() == true){
            distance += 0.0f;//10
            sensorsOn += 1;
        }
        if (sen4.sensorState() == true) {
            distance -= 0.0f;//5
            sensorsOn += 1;
        }
        if (sen5.sensorState() == true) {
            distance -= weight;//19
            sensorsOn += 1;
        }
        if (sen6.sensorState() == true){
            distance -= 0.0f;//33
            sensorsOn += 0;
        }
        if(sen3.sensorState() == true && sen4.sensorState() == true){
            distance = 0.0f;
            global_ang_speed_left = DEFAULT_SPEED;
            global_ang_speed_right = DEFAULT_SPEED;
        }
        if (distance != 0.0f) step = 0.1;
        else step = 0.02;
        if (sensorsOn > 0) {
            distance = distance/sensorsOn;
            lcd.locate(50,0);
            lcd.printf("%.0f",distance);
            disable_speed_controls = false;
            wheel_right->line();
        }
        /*
        else {//No line
            lcd.locate(50,0);
            lcd.printf("NoL");
            disable_speed_controls = true;
            wheel_right->no_line();
        }
        */
        float P_ = distance*Kp; //creates the p value by a given error(distance) and the controlling variable kp
        float output = P_; //for future if I_ and D_ are implemented
        if (output > 0) { //line is on the left hand side
            global_ang_speed_left -= output;//speeds up right wheel and slows left
            global_ang_speed_right += output;
        } else if (output < 0) { //line is on the right hand side
            global_ang_speed_left -= output;//slows right wheel and speeds up left
            global_ang_speed_right += output;
        } else {
//            global_ang_speed_left = DEFAULT_SPEED;
//            global_ang_speed_right = DEFAULT_SPEED;//remains constant forward
        }
//Safeguards so that speed doesn't go to negatives
        if (global_ang_speed_left < 0.0f)global_ang_speed_left = 0.0f;
        if (global_ang_speed_right < 0.0f)global_ang_speed_right = 0.0f;
//Speed controller
        if(disable_speed_controls == false) {
            if(wheel_left->get_ang_speed() > global_ang_speed_left)car.changeDutyCycle(0.0f, step);
            else if(wheel_left->get_ang_speed() < global_ang_speed_left)car.changeDutyCycle(0.0f, -1.0f*step);
            if(wheel_right->get_ang_speed() > global_ang_speed_right)car.changeDutyCycle(step, 0.0f);
            else if(wheel_right->get_ang_speed() < global_ang_speed_right)car.changeDutyCycle(-1.0f*step, 0.0f);
        }
        
        //Data for troubleshooting
        lcd.locate(0,0);
        lcd.printf("Kp: %.3f",Kp);
        lcd.locate(80,0);
        lcd.printf("D:%.2f", output);
        lcd.locate(50,10);
        lcd.printf("Weight: %.1f",weight);
        lcd.locate(0,10);
        lcd.printf("%.1lf||%.1lf",wheel_left->get_ang_speed(), wheel_right->get_ang_speed());
        lcd.locate(0,20);
        lcd.printf("%.1lf||%.1lf",global_ang_speed_left, global_ang_speed_right);
        lcd.locate(50,20);
        lcd.printf("DC:%.2f||%.2f",car.get_curr_dc(LEFT),car.get_curr_dc(RIGHT));
    }
}
