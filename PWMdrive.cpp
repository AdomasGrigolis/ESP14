//In main, use Car car;
//Then to call functions, use car.XX(); or int integer car->get.XX;
#include "mbed.h"
#define FREQUENCY 18000.0f//Above audible range
#define STATIONARY_DUTY_CYCLE 0.5f
#define STARDARD_MOVEMENT_SPEED 0.5f
//Pins
#define ENABLE_PIN PA_13
#define BIPOLAR1_PIN PH_1
#define BIPOLAR2_PIN PH_0
#define PWM_PIN1 PB_7
#define PWM_PIN2 PA_15
#define PERIOD 1.0f/FREQUENCY//Period for PWM signal

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
        PwmOut Motor_1;
        PwmOut Motor_2;
public:
    Car(PinName enable = ENABLE_PIN, PinName bipolar1 = BIPOLAR1_PIN, PinName bipolar2 = BIPOLAR2_PIN, PinName pwm1 = PWM_PIN1, PinName pwm2 = PWM_PIN2) :
     Enable_Driveboard(enable), Bipolar_1(bipolar1), Bipolar_2(bipolar2), Motor_1(pwm1), Motor_2(pwm2)
    {   
        //Enable the motor driveboard
        Enable_Driveboard = 1;
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
            break;
        case RIGHT:
            this->setMotorSpeeds(STARDARD_MOVEMENT_SPEED, 0.0f);
            wait(0.1);
            break;
        }
    }
    void turn180Degrees()
    {
       setMotorSpeeds(1.0f, 0.0f);
            wait(0.1); 
    }
};
