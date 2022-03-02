#include "mbed.h"
#include <encoders.cpp>
#include <PWMdrive.cpp>
#define BUGGY_WIDTH 190//distance between two wheels of buggy is 190 mm
#define Pi 3.1415
//turning_circule perimeter for 360 degrees
const float TURNING_CIRCLE = Pi*BUGGY_WIDTH*2.00;
//arc length for 90 degrees
const float quarter_circle = TURNING_CIRCLE/4.00;
//arc length for 180 degress
const float half_circle = TURNING_CIRCLE/2.00;

Car car;
float volatile curr_pos;

int main(void){
    TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
    TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
    volatile int counter = 0;
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
                if(curr_pos> 500)
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
                else if(counter > 4)
                    state = turning_right;
                else if(counter == 8)
                    state = finish;
                break;    
            }
            case turning_right:
            {   
                car.turn90Degrees(RIGHT);
                if(wheel_left->get_dx() > quarter_circle)
                    state = driving_initialisation;
                break; 
            }
            case turning_left:
            {
                car.turn90Degrees(LEFT);
                if(wheel_right->get_dx() > quarter_circle)
                    state = driving_initialisation;
                break; 
            }
            case turning180:
            {
                car.turn180Degrees();
                if(wheel_left->get_dx() > half_circle) 
                break;    
            }
            case finish:
            {
                car.stop();
                break; 
            }
            default: 
            {
                state = stop;
                break;
            }
        }
    }
}
