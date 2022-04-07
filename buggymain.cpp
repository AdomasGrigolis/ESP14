#include "mbed.h" //includes mbed library
#include "C12832.h" //includes LCD screen lib
#include <encoder.cpp>
#include <PWMdrive.cpp>
#include <pot.cpp>
#include <sensor.cpp>

#define STOPPING_STIFFNESS 0.01f
#define STOPPING_SPEED 0.1f

//Desired angular speeds
double global_ang_speed_right = 7.0;
double global_ang_speed_left = 7.0;

// Max X value of LCD screen
// Max Y value of LCD screen

TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
Potentiometer* leftPot = new SamplingPotentiometer(A0, 3.3, 100.0); 
Car car;//Motor control
C12832 lcd(D11, D13, D12, D7, D10);
//Line sensors
SENSOR sen1(PC_4); //grey
SENSOR sen2(PC_0);//blue
SENSOR sen3(PC_5);//green
SENSOR sen4(PC_2);//red
SENSOR sen5(PC_3);//purple
SENSOR sen6(PB_0);//orange Power drop on this pin because it is shared with the joystick.

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
    decc_tick.attach(deceleration, STOPPING_SPEED);
}//Deceleration end

//Controllers
Ticker sp_tick;
void speedController(void){//Speed control
    if(wheel_left->get_ang_speed() > global_ang_speed_left)car.changeDutyCycle(0.05f, 0.0f);
    else if(wheel_left->get_ang_speed() < global_ang_speed_left)car.changeDutyCycle(-0.05f, 0.0f);
    if(wheel_right->get_ang_speed() > global_ang_speed_right)car.changeDutyCycle(0.0f, 0.05f);
    else if(wheel_left->get_ang_speed() < global_ang_speed_right)car.changeDutyCycle(0.0f, -0.05f);
}

float calculate_PID(float error_in, float kp){
    float P_ = error_in *kp; //creates the p value by a given error(distance) and the controlling variable kp
    float duty = 0.7f; //default crusing speed
    float output = P_; //for future if I_ and D_ are implemented 
    
    if (output < 0){ //line is on the left hand side
        car.setMotorSpeeds(duty + P_, duty - P_); //decreases duty cycle 1(right) and increases duty 2 (left)
                                                  //this speeds up the right wheel and slows the left
    }
    else if (output > 0){
        car.setMotorSpeeds(duty + P_, duty - P_); //increases duty1 (right) decreases duty2(left)
                                                  //slows right wheel and speeds up the left
    }
    else{
        car.setMotorSpeeds(duty, duty);//remains constant forward
        
    }
    return output; //returns the output for trouble shooting
}

int main(void)
{
    lcd.set_auto_up(1);//This ensures no flickering on LCD
    float Kp;
    int sensorsOn;
    float distance;
    float volatile duty_delta;
    sp_tick.attach(speedController, 0.01);
    while(1) {
        Kp = leftPot->getCurrentSampleNorm()/4.0f;//Up to 0.25
        lcd.locate(0,0);
        lcd.printf("%.3f",Kp);
        lcd.locate(0,10);
        lcd.printf("%.1lf||%.1lf",wheel_left->get_ang_speed(), wheel_right->get_ang_speed());
//        led = 1;
        distance = 0.0;
        sensorsOn = 0;
        if (sen1.sensorState() == true){
            distance += 45;
            sensorsOn += 1;
        }
//        else if (sen1.sensorState() == false){}
        if (sen2.sensorState() == true){
            distance += 25;
            sensorsOn += 1;
        }
//        else if (sen2.sensorState() == false){}
        if (sen3.sensorState() == true){
            distance += 10;
            sensorsOn += 1;
        }
//        else if (sen3.sensorState() == false){}
        if (sen4.sensorState() == true){
            distance -= 10;
            sensorsOn += 1;
        }
//        else if (sen4.sensorState() == false){}
        if (sen5.sensorState() == true){
            distance -= 25;
            sensorsOn += 1;
        }
//        else if (sen5.sensorState() == false){}
        if (sen6.sensorState() == true){
            distance -= 45;
            sensorsOn += 1;
        }
//        else if (sen6.sensorState() == false){   }
        if (sensorsOn > 0){
            distance = distance / sensorsOn;
            lcd.locate(50,0);
            lcd.printf("%.0f",distance);
            duty_delta = calculate_PID(distance, Kp);
            wheel_right->line();
        }
        else{
            lcd.locate(50,0);
            lcd.printf("No");
            lcd.locate(50,10);
            lcd.printf("line");
            wheel_right->no_line(); 
        }
    }
}
