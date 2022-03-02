#include "mbed.h"
#define FREQUENCY 18000.0f//Above audible range
#define STATIONARY_DUTY_CYCLE 0.5f
#define STARDARD_MOVEMENT_SPEED 0.5f
//Pins
#define ENABLE_PIN_OUR PA_13//Do not use ENABLE_PIN as it's already used in mbed
#define BIPOLAR1_PIN PH_1
#define BIPOLAR2_PIN PH_0
#define PWM_PIN1 PB_7
#define PWM_PIN2 PA_15

double PERIOD = 1.0f/FREQUENCY;//Period for PWM signal
//Enable the Motor Driveboard
DigitalOut Enable_Driveboard(ENABLE_PIN_OUR);

//Use bipolar mode
DigitalOut Bipolar_1(BIPOLAR1_PIN);
DigitalOut Bipolar_2(BIPOLAR2_PIN);

PwmOut Motor_1(PWM_PIN1);
PwmOut Motor_2(PWM_PIN2);

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

public:
    Car()
    {
        Enable_Driveboard = 1;//Enable the motor driveboard
        //Use Bipolar mode
        Bipolar_1 = 1;
        Bipolar_2 = 1;
        //For A stationary motor requires exactly 50% duty cycle.
        Motor_1.period(PERIOD);
        Motor_2.period(PERIOD);
        setMotorSpeeds(0.0, 0.0);
    }

    // -1 goes backwards as fast as possible  == 0.0 duty cycle
    // 1 goes forwards as fast as possible    == 1.0 duty cycle
    // 0 stalls                               == 0.5 duty cycle  
    void setMotorSpeeds(float new_motor1_speed, float new_motor2_speed)
    {
        duty_cycle_1 = this->convertSpeedToDutyCycle(new_motor1_speed);
        Motor_1.write(duty_cycle_1);
        duty_cycle_2 = this->convertSpeedToDutyCycle(new_motor2_speed);
        Motor_2.write(duty_cycle_2);
    }

    float convertSpeedToDutyCycle(float speed)
    {
        return speed * STATIONARY_DUTY_CYCLE + STATIONARY_DUTY_CYCLE;
    }
    
    void startMovingForward()
    {
        this->setMotorSpeeds(STARDARD_MOVEMENT_SPEED, STARDARD_MOVEMENT_SPEED);
    }

    void stop()
    {
        this->setMotorSpeeds(0.0f, 0.0f);
    }

    void turn90Degrees(Turn t)
    {
        switch(t){
        case LEFT:
            this->setMotorSpeeds(0.0f, STARDARD_MOVEMENT_SPEED);
            wait(0.1);
            this->startMovingForward();
            break;
        case RIGHT:
            this->setMotorSpeeds(STARDARD_MOVEMENT_SPEED, 0.0f);
            wait(0.1);
            this->startMovingForward();
            break;
        }
    }
    void turn180Degrees()
    {
        this->setMotorSpeeds(1.0f, -1.0f);
        wait(0.1);
        this->startMovingForward();
    }
};
