//    Car car();
//    car.startMovingForward();
#include "mbed.h"
#define FREQUENCY 18000.0f
#define STATIONARY_DUTY_CYCLE 0.5f
#define STARDARD_MOVEMENT_SPEED 0.5f
#define ENABLE_PIN
#define BIPOLAR1_PIN
#define BIPOLAR2_PIN
float PERIOD = 1.0/FREQUENCY;
//Enable the Motor Driveboard
DigitalOut Enable_Driveboard(ENABLE_PIN);

//Use bipolar mode
DigitalOut Bipolar_1(BIPOLAR1_PIN);
DigitalOut Bipolar_2(BIPOLAR2_PIN);
DigitalOut Direction_1(D0);
DigitalOut Direction_2(D1);

PwmOut Motor_1(D5);
PwmOut Motor_2(D6);

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
        //Enable the motor driveboard
        Enable_Driveboard = 1;
        //Use Bipolar mode
        Bipolar_1 = 1;
        Bipolar_2 = 1;
        wait(0.4);
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
        duty_cycle_1 = convertSpeedToDutyCycle(new_motor1_speed);
        Motor_1.write(duty_cycle_1);
        duty_cycle_2 = convertSpeedToDutyCycle(new_motor2_speed);
        Motor_2.write(duty_cycle_2);
    }

    float convertSpeedToDutyCycle(float speed)
    {
        return speed * STATIONARY_DUTY_CYCLE + STATIONARY_DUTY_CYCLE;
    }
    
    void startMovingForward()
    {
        setMotorSpeeds(STARDARD_MOVEMENT_SPEED, STARDARD_MOVEMENT_SPEED);
    }

    void stop()
    {
        setMotorSpeeds(0.0f, 0.0f);
    }

    void turn90Degrees(Turn t)
    {
        switch(t)
        case LEFT:
            setMotorSpeeds(0.0f, STARDARD_MOVEMENT_SPEED);
            wait(0.1);
            startMovingForward();
            break;
        case RIGHT:
            setMotorSpeeds(STARDARD_MOVEMENT_SPEED, 0.0f);
            wait(0.1);
            startMovingForward();
            break;
    }
};
