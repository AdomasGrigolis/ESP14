#include "mbed.h"
#include <QEI.h>
#include <encoders.cpp>
#define TURNING_CIRCLE 
int main(void){
    volatile int counter = 0;
    typedef enum {driving_initialisation, driving, stop, turning_initialisation, turning, finish} BuggyState;
    BuggyState state = driving_initialisation;
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
            {   //Motor_1.duty_cycle()=0.2;
                //Motor_2.duty_cycle()=0.2;
                Car.setMotorSpeeds(0.2f, 0.2f);
                curr_pos=(wheel_left.get_dx()+wheel_right.get_dx())/2;
                if(curr_pos> 500)
                    state = stop;
                break;    
            }
            case stop:
            {   
                //Motor_1.duty_cycle()=0.5;
                //Motor_2.duty_cycle()=0.5;
                Car.stop();
                state = turning_initialisation;
                break;    
            }
            case turning_initialisation:
            {
                wheel_left.zero_dx();
                wheel_right.zero_dx();
                counter++;
                if(counter==4)
                    state = turning180;
                else if(counter == 8)
                    state = finish; 
                state = driving;
                break;    
            }
            case turning:
            {   Car.turn90Degrees(t);
                if(wheel_left.get_dx() > TURNING_CIRCLE)
                    state = driving_initialisation;
                break; 
            }
            case finish;
            {
                Car.stop();
            }
            default:
                break;
    }