#include "mbed.h"
#include "QEI.h"

//Enable the Motor Driveboard
DigitalOut Enable_Driveboard(PA_8);

//Use bipolar mode
DigitalOut Bipolar_1(PB_10);
DigitalOut Bipolar_2(PB_4);
//PWM Out for motors
PwmOut Motor_1(PB_5);
PwmOut Motor_2(PB_3);
//Global Variable
float period = 4;
float volatile duty_cycle_1 = 0.25f;
float volatile duty_cycle_2 = 0.25f;

int main() {
 //Enable the motor driveboard
    Enable_Driveboard = 0;
    //Use Bipolar mode
    Bipolar_1 = 0;
    Bipolar_2 = 0;
    //For A stationary motor requires exactly 50% duty cycle.
    Motor_1.period_ms(period); // 0.4 second period
    Motor_1.write(duty_cycle_1); // 25% duty cycle, relative to period
    Motor_2.period_ms(period); // 0.4 second period
    Motor_2.write(duty_cycle_2); // 25% duty cycle, relative to period
    
 while(1);
}
