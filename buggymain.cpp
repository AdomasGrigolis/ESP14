#include "mbed.h" //includes mbed library
#include "C12832.h" //includes LCD screen lib
#include <encoders.cpp>
#include <PWMdrive.cpp>
#include <pot.cpp>

// Max X value of LCD screen

// Max Y value of LCD screen
TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
Potentiometer* leftPot = new SamplingPotentiometer(A0, 3.3, 100.0);

Car car;
C12832 lcd(D11, D13, D12, D7, D10);     //Creates an LCD Object from the LCD library
DigitalOut led(PB_2);//yellow
float threshold = 0.5;
class SENSOR{
    private:
        AnalogIn inputSignal;
    public:
        SENSOR(PinName pin) : inputSignal(pin){;}
        
        bool sensorState(void){

            float voltage = inputSignal.read() *5;
            if (voltage >= threshold){
                return true;
            }
            else{
                return false;   
            }
        }
    
      
};
SENSOR sen1(PC_4); //grey
SENSOR sen2(PC_0);//blue
SENSOR sen3(PC_5);//green
SENSOR sen4(PC_2);//red
SENSOR sen5(PC_3);//purple
SENSOR sen6(PB_0);//orange Power drop on this pin because it is shared with the joystick.



float calculate_PID(float error_in, float kp){
    float P_ = error_in *kp; //creates the p value by a given error(distance) and the controlling variable kp
    float duty = 0.6; //default crusing speed
    float output = P_; //for future if I_ and D_ are implemented 
    
    if (output < 0){ //line is on the left hand side
        car.setMotorSpeeds(duty - P_, duty + P_); //decreases duty cycle 1(right) and increases duty 2 (left)
                                                  //this speeds up the right wheel and slows the left
    }
    else if (output > 0){
        car.setMotorSpeeds(duty - P_, duty + P_); //increases duty1 (right) decreases duty2(left)
                                                  //slows right wheel and speeds up the left
    }
    else{
        car.setMotorSpeeds(duty, duty);//remains constant forward
        
    }
    return output; //returns the output for trouble shooting
}



int main()
{
    float Kp;
    int sensorsOn;
    float distance;
    float duty_delta;
    double angular_speed_left = 0.0;
    double angular_speed_right = 0.0;
    car.startMovingForward();
    while(1) {
        Kp = (leftPot->getCurrentSampleNorm())/100;
        led = 1;
        distance = 0.0;
        sensorsOn = 0;
        
        // speed control
       // angular_speed_left = wheel_left->get_ang_speed();
       // angular_speed_right = wheel_right->get_ang_speed();
       // calculate_PID_speed(angular_speed_left,angular_speed_right,0.6);
        
        if (sen1.sensorState() == true){
            lcd.locate(20,0);
            lcd.printf("1: on");
            distance += 45;
            sensorsOn += 1;
        }
        else if (sen1.sensorState() == false){
            lcd.locate(20,0);
            lcd.printf("1: off");
            
        }
        if (sen2.sensorState() == true){
            lcd.locate(20,10);
            lcd.printf("2: on");
            distance += 25;
            sensorsOn += 1;
        }
        else if (sen2.sensorState() == false){
            lcd.locate(20,10);
            lcd.printf("2: off");
        }
        if (sen3.sensorState() == true){
            lcd.locate(20,20);
            lcd.printf("3: on");
            distance += 10;
            sensorsOn += 1;
        }
        else if (sen3.sensorState() == false){
            lcd.locate(20,20);
            lcd.printf("3: off");
        }
        if (sen4.sensorState() == true){
            lcd.locate(80,0);
            lcd.printf("4: on");
            distance -= 10;
            sensorsOn += 1;
        }
        else if (sen4.sensorState() == false){
            lcd.locate(80,0);
            lcd.printf("4: off");
        }
        if (sen5.sensorState() == true){
            lcd.locate(80,10);
            lcd.printf("5: on");
            distance -= 25;
            sensorsOn += 1;
        }
        else if (sen5.sensorState() == false){
            lcd.locate(80,10);
            lcd.printf("5: off");
        }
        if (sen6.sensorState() == true){
            lcd.locate(80,20);
            lcd.printf("6: on");
            distance -= 45;
            sensorsOn += 1;
        }
        else if (sen6.sensorState() == false){
            lcd.locate(80,20);
            lcd.printf("6: off");
            
        }
        if (sensorsOn > 0){
            distance = distance / sensorsOn;
            lcd.locate(40,0);
            lcd.printf("%.3f",Kp);
            duty_delta = calculate_PID(distance, Kp);
            wheel_left->line();
        }
        else{
            lcd.locate(40,0);
            lcd.printf("%.5f",Kp);
            wheel_left->no_line();     
        }
    }
}
