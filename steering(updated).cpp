#include "mbed.h"
#include <QEI.h>

const float ENCODER_MEASURE_PERIOD = 1.0f;//Encoder pulses are checked every 1 second
const int NO_PULS_PER_REV = 1024;//Pulses per revolution (256*4)
const float WHEEL_CIRCUMFERENCE = 276.46f;//In milimetres
const float DISTANCE_between_WHEELS = // In milimetres 
const float TURNING_CIRCLE = 
volatile double R_dx = 0;//Wheel linear distance driven mm
volatile double L_dx = 0;
volatile double R_speed = 0;//Wheel linear speed (constant period) mm/s
volatile double L_speed = 0;

QEI* wheel_left = new TickingEncoder(D8, D9, NC);
QEI* wheel_right = new TickingEncoder(D10, D11, NC);

int main(void){

    typedef enum {driving_initialisation, driving, stop, turning_initialisation, turning} BuggyState;
    BuggyState state = initialisation;
    
    while (1)
    {
        switch (state)
        {
            case driving_initialisation:
            {
                wheel_left.zero_dx();
                wheel_right.zero_dx();
                state = driving;
                break;
            }
            case driving:
            {   Motor_1.duty_cycle()=0.2;
				Motor_2.duty_cycle()=0.2;
                curr_pos=(wheel_left.get_dx()+wheel_right.get_dx())/2;
                if(curr_pos> 500)
                    state = stop;
                break;    
            }
            case stop:
            {   
                Motor_1.duty_cycle()=0.5;
				Motor_2.duty_cycle()=0.5;
                state = turning_initialisation;
                break;    
            }
            case turning_initialisation:
            {
                wheel_left.zero_dx();
                wheel_right.zero_dx();
                break;    
            }
            case turning:
            {   Motor_1.duty_cycle()=0.2;
				Motor_2.duty_cycle()=0.8;
                if(wheel_left.get_dx() > TURNING_CIRCLE)
                    state = driving_initialisation;
                break; 
            }
            default:
                break;
    }