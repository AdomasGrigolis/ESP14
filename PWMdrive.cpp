#pragma once
//In main, use Car car; to create an obj
//Then to call functions, use car.XX(); or int integer car->get.XX;
#include "mbed.h"
#define FREQUENCY 10000.0f//Above audible range
#define PERIOD 1.0f/FREQUENCY//Period for PWM signal
//Increments less than 0.01 should not be used under any circumstance
#define STATIONARY_DUTY_CYCLE 1.0f
#define STANDARD_MOVEMENT_SPEED 0.7f//0.7 is found to be suitable for slow cruizing
#define RIGHT_OFFSET 0.03f//This offset allows to balance out left and right 
#define STANDARD_TURNING_SPEED 0.75f
#define ACC_STIFFNESS 0.01f//Acceleration increments, higher the number, faster the change
//Pins
#define ENABLE_PIN PA_13
#define BIPOLAR1_PIN PH_1
#define BIPOLAR2_PIN PH_0
#define DIR_PIN1 PC_14
#define DIR_PIN2 PA_14
#define PWM_PIN1 PB_1
#define PWM_PIN2 PC_8
#define FORWARD 0
#define BACKWARD 1

enum Turn
{
    LEFT, RIGHT
};

class Car
{
private: 
        DigitalOut Enable_Driveboard;
        PwmOut Motor_1;
        PwmOut Motor_2;
        DigitalOut Bipolar_1;
        DigitalOut Bipolar_2;
        DigitalOut Direction_1;
        DigitalOut Direction_2;
        float curr_dc_1;
        float curr_dc_2;
public:
    Car(PinName enable = ENABLE_PIN, PinName pwm1 = PWM_PIN1, PinName pwm2 = PWM_PIN2, 
     PinName bipolar1 = BIPOLAR1_PIN, PinName bipolar2 = BIPOLAR2_PIN, PinName dir1 = DIR_PIN1, PinName dir2 = DIR_PIN2) :
     Enable_Driveboard(enable), Motor_1(pwm1), Motor_2(pwm2),
     Bipolar_1(bipolar1), Bipolar_2(bipolar2), Direction_1(dir1), Direction_2(dir2)
    {   
        Enable_Driveboard = 1;
        Motor_1.period(PERIOD);
        Motor_2.period(PERIOD);
        //Use Unipolar mode
        Bipolar_1 = 0;
        Bipolar_2 = 0;
        //Default direction is forward
        Direction_1 = FORWARD;
        Direction_2 = FORWARD;
        stop();//Init at stall
    }
    void setMotorSpeeds(float duty_cycle_1, float duty_cycle_2)
    {//Main function that writes DC directly to output
        curr_dc_1 = duty_cycle_1;
        curr_dc_2 = duty_cycle_2;
        //if (!(duty_cycle_1 >= 1.0f))duty_cycle_1 -= RIGHT_OFFSET;
        Motor_1.write(duty_cycle_1);
        Motor_2.write(duty_cycle_2);
    }
//Moving linearly    
    void startMovingForward()
    {
        setDirectionForward();
        setMotorSpeeds(STANDARD_MOVEMENT_SPEED, STANDARD_MOVEMENT_SPEED);
    }
    void startMovingBackward()
    {
        setDirectionBackward();
        setMotorSpeeds(STANDARD_MOVEMENT_SPEED, STANDARD_MOVEMENT_SPEED);
    }
    void startMoving()//Direction should be set beforehand or will be forward by default
    {
        setMotorSpeeds(STANDARD_MOVEMENT_SPEED, STANDARD_MOVEMENT_SPEED);
    }
//Setting direction
    void setDirectionForward()
    {
        Direction_1 = FORWARD;
        Direction_2 = FORWARD;
    }
    void setDirectionBackward()
    {
        Direction_1 = BACKWARD;
        Direction_2 = BACKWARD;
    }
//Stopping capabilities
    void stop()
    {
        setMotorSpeeds(STATIONARY_DUTY_CYCLE, STATIONARY_DUTY_CYCLE);
    }
//Turns
    void turnOneWheel(Turn t)//Turns with one wheel stationary
    {
        switch(t){
        case LEFT:
            setMotorSpeeds(STATIONARY_DUTY_CYCLE, STANDARD_MOVEMENT_SPEED);
            Direction_1 = FORWARD;
            break;
        case RIGHT:
            setMotorSpeeds(STANDARD_MOVEMENT_SPEED, STATIONARY_DUTY_CYCLE);
            Direction_2 = FORWARD;
            break;
        }
    }
    void turnaround(Turn t = LEFT)//Turns with both wheels in opposite directions
    {
        switch(t){
        case LEFT:
            Direction_1 = BACKWARD;
            Direction_2 = FORWARD;
            break;
        case RIGHT:
            Direction_1 = FORWARD;
            Direction_2 = BACKWARD;
            break;
        }
        setMotorSpeeds(STANDARD_TURNING_SPEED, STANDARD_TURNING_SPEED);
    }
    float get_curr_dc(Turn t){
        switch(t){
        case LEFT:
            return curr_dc_2;
        case RIGHT:
            return curr_dc_1;
        }
        return -1.0f;
    }
};//End of class definition
