#include "mbed.h"
//Enable the Motor Driveboard
DigitalOut Enable_Driveboard(PA_8);
//Use bipolar mode
DigitalOut Bipolar_1(PB_10);
DigitalOut Bipolar_2(PB_4);
//PWM Out for motors
PwmOut Motor_1(PB_5);
PwmOut Motor_2(PB_3);


int main() {
    //Enable the motor driveboard
    Enable_Driveboard = 1;
    //Use Bipolar mode
    Bipolar_1 = 1;
    Bipolar_2 = 1;
    //For A stationary motor requires exactly 50% duty cycle.
    Motor_1.period(0.4f); // 0.4 second period
    Motor_2.period(0.4f); // 0.4 second period
    Motor_1.write(0.10f); // 25% duty cycle, relative to period
    Motor_2.write(0.10f); // 25% duty cycle, relative to period
 while(1);
}