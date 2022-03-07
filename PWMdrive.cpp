//In main, use Car car;
//Then to call functions, use car.XX(); or int integer car->get.XX;
#include "mbed.h"
#define FREQUENCY 10000.0f//Above audible range
#define STATIONARY_DUTY_CYCLE 1.0
#define STARDARD_MOVEMENT_SPEED 0.6
#define STARDARD_MOVEMENT_SPEED_RIGHT 0.65
#define STARDARD_TURNING_SPEED 0.65
//Pins
#define ENABLE_PIN PA_13
#define BIPOLAR1_PIN PH_1
#define BIPOLAR2_PIN PH_0
#define DIR_PIN1 PC_14
#define DIR_PIN2 PA_14
#define PWM_PIN1 PB_1
#define PWM_PIN2 PC_8
#define PERIOD 1.0f/FREQUENCY//Period for PWM signal
#define FORWARD 0
#define BACKWARD 1

enum Turn
{
    LEFT, RIGHT
};

class Car
{
private: 
        //PWM Out for motors
        float duty_cycle_1;//0.5 is stall
        float duty_cycle_2;
        DigitalOut Enable_Driveboard;
        DigitalOut Bipolar_1;
        DigitalOut Bipolar_2;
        DigitalOut Direction_1;
        DigitalOut Direction_2;
        PwmOut Motor_1;
        PwmOut Motor_2;
public:
    Car(PinName enable = ENABLE_PIN, PinName bipolar1 = BIPOLAR1_PIN, PinName bipolar2 = BIPOLAR2_PIN, PinName pwm1 = PWM_PIN1, PinName pwm2 = PWM_PIN2, PinName dir1 = DIR_PIN1, PinName dir2 = DIR_PIN2) :
     Enable_Driveboard(enable), Bipolar_1(bipolar1), Bipolar_2(bipolar2), Motor_1(pwm1), Motor_2(pwm2), Direction_1(dir1), Direction_2(dir2)
    {   
        //Enable the motor driveboard
        Enable_Driveboard = 1;
        //Use Bipolar mode
        Bipolar_1 = 0;
        Bipolar_2 = 0;
        Motor_1.period(PERIOD);
        Motor_2.period(PERIOD);
        stop();//Init at stall
    }
    void setMotorSpeeds(float duty_cycle_1, float duty_cycle_2)
    {
        Motor_1.write(duty_cycle_1);
        Motor_2.write(duty_cycle_2);
    }
    
    void startMovingForward()
    {
        setMotorSpeeds(STARDARD_MOVEMENT_SPEED, STARDARD_MOVEMENT_SPEED);
        Direction_1 = FORWARD;
        Direction_2 = FORWARD;
    }
    void startMovingBackward()
    {
        setMotorSpeeds(STARDARD_MOVEMENT_SPEED, STARDARD_MOVEMENT_SPEED);
        Direction_1 = BACKWARD;
        Direction_2 = BACKWARD;
    }

    void stop()
    {
        setMotorSpeeds(STATIONARY_DUTY_CYCLE, STATIONARY_DUTY_CYCLE);
        wait(0.1);
    }

    void turn90Degrees(Turn t)
    {
        switch(t){
        case LEFT:
            setMotorSpeeds(STATIONARY_DUTY_CYCLE, STARDARD_MOVEMENT_SPEED);
            Direction_1 = FORWARD;
            wait(0.1);
            break;
        case RIGHT:
            setMotorSpeeds(STARDARD_MOVEMENT_SPEED, STATIONARY_DUTY_CYCLE);
            Direction_2 = FORWARD;
            wait(0.1);
            break;
        }
    }
    void turn180Degrees()
    {
       setMotorSpeeds(STARDARD_MOVEMENT_SPEED, STARDARD_MOVEMENT_SPEED);
            Direction_1 = FORWARD;
            Direction_2 = BACKWARD;
            wait(0.1); 
    }
};
