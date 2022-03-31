#include "mbed.h"
#include "C12832.h"
#include <encoder.cpp>//Encoder data
#include <PWMdrive.cpp>//Motor control

#define BUGGY_WIDTH 195//distance between two wheel centres in mm
#define Pi 3.1415f
#define OVERSHOOT_DIST 75.0f//In mm
#define OVERSHOOT_DIST_TURN 60.0f//In mm
#define OVERSHOOT_DIST_TURNAROUND 80.0f//In mm

//turning_circule perimeter for 360 degrees
const float TURNING_CIRCLE = Pi*BUGGY_WIDTH*2.0f;
//arc length for 90 degrees
const float quarter_circle = TURNING_CIRCLE/4.00f;
//arc length for 180 degress
const float half_circle = TURNING_CIRCLE/2.00f;

C12832 lcd(D11, D13, D12, D7, D10);//LCD interface
Car car;//Object to control motors

//Encoder objects pull data and perform operations to return speed and distance
TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);

void screen_update(){
        lcd.locate(0,0);
        lcd.printf("RDist: %0.3lf", wheel_right->get_dx());
        lcd.locate(0,10);
        lcd.printf("LDist: %0.3lf", wheel_left->get_dx());
}

int main(void){
    float curr_pos;//Current position of the buggy
    int counter = 0;//Counter for performing turns
    //Screen will output data from encoders
    lcd.cls(); 
    lcd.set_auto_up(1);//Makes LCD not flicker by updating only when necessary
    //State machine program to perform a square
    typedef enum {driving_initialisation, driving, stop, turning_initialisation, turning_right, turning_left, turning180, finish} BuggyState;
    BuggyState state = driving_initialisation;
    while (1)
    {
        switch (state)
        {
            case driving_initialisation:
            {
                wheel_left->zero_dx();
                wheel_right->zero_dx();
                state = driving;
                break;
            }
            case driving:
            {   
                car.startMovingForward();
                curr_pos=(wheel_left->get_dx()+wheel_right->get_dx())/2;
                screen_update();
                if(curr_pos >= (500-OVERSHOOT_DIST))
                    state = stop;
                break;    
            }
            case stop:
            {   
                car.stop();
                state = turning_initialisation;
                break;    
            }
            case turning_initialisation:
            {
                wheel_left->zero_dx();
                wheel_right->zero_dx();
                counter++;
                if(counter < 4)
                    state = turning_left;
                else if(counter==4)
                    state = turning180;
                else if((counter > 4) && (counter < 8))
                    state = turning_right;
                else if(counter == 8)
                    state = finish;
                break;    
            }
            case turning_right:
            {   
                car.turn90Degrees(RIGHT);
                if(wheel_right->get_dx() > (quarter_circle - OVERSHOOT_DIST_TURN))
                    state = driving_initialisation;
                break; 
            }
            case turning_left:
            {
                car.turn90Degrees(LEFT);
                if(wheel_left->get_dx() > (quarter_circle - OVERSHOOT_DIST_TURN))
                    state = driving_initialisation;
                break; 
            }
            case turning180:
            {
                car.turn90Degrees(LEFT);
                if(wheel_left->get_dx() > (half_circle - OVERSHOOT_DIST_TURNAROUND))
                    state = driving_initialisation; 
                break;    
            }
            case finish:
            {
                car.stop();
                break; 
            }
            default: 
            {
                state = finish;
                break;
            }
        }
    }
}
