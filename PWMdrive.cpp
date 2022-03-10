//In main, use Car car; to create an obj
//Then to call functions, use car.XX(); or int integer car->get.XX;
#include "mbed.h"
#define FREQUENCY 10000.0f//Above audible range
#define PERIOD 1.0f/FREQUENCY//Period for PWM signal
//Increments less than 0.01 should not be used under any circumstance
#define STATIONARY_DUTY_CYCLE 1.0f
#define STARDARD_MOVEMENT_SPEED 0.7f//0.7 is found to be suitable for slow cruizing
#define STARDARD_TURNING_SPEED 0.75f
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
        enum Turn
        {
            LEFT, RIGHT
        };
        //Wheel independent acceleration isrs and related variables
        Ticker acc_tick;
        Ticker decc_tick;
        float curr_dc_1;
        float curr_dc_2;
        void acceleration()//Isr called to cause acceleration
        {
            if (curr_dc_1 > STARDARD_MOVEMENT_SPEED)
            {
                curr_dc_1 -= ACC_STIFFNESS;
                setMotorSpeeds(curr_dc_1, curr_dc_2);
            }
            if (curr_dc_2 > STARDARD_MOVEMENT_SPEED)
            {
                curr_dc_2 -= ACC_STIFFNESS;
                setMotorSpeeds(curr_dc_1, curr_dc_2);
            }
            if (curr_dc_1 <= STARDARD_MOVEMENT_SPEED && curr_dc_2 <= STARDARD_MOVEMENT_SPEED)acc_tick.detach();
        }
        void deceleration()//Isr called to cause acceleration
        {
            if (curr_dc_1 < STATIONARY_DUTY_CYCLE)
            {
                curr_dc_1 += ACC_STIFFNESS;
                setMotorSpeeds(curr_dc_1, curr_dc_2);
            }
            if (curr_dc_2 < STATIONARY_DUTY_CYCLE)
            {
                curr_dc_2 += ACC_STIFFNESS;
                setMotorSpeeds(curr_dc_1, curr_dc_2);
            }
            if (curr_dc_1 >= STATIONARY_DUTY_CYCLE && curr_dc_2 >= STATIONARY_DUTY_CYCLE)decc_tick.detach();
        }
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
        curr_dc_1 = duty_cycle_1;
        curr_dc_2 = duty_cycle_2;
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
    }
    
    void accelerate()
    {
        decc_tick.detach();
        acc_tick.attach(callback(this, &Car::acceleration), 0.1);
    }
    void decelerate()
    {
        acc_tick.detach();
        decc_tick.attach(callback(this, &Car::deceleration), 0.1);
    }

    void turnOneWheel(Turn t)//Turns with one wheel stationary
    {
        switch(t){
        case LEFT:
            setMotorSpeeds(STATIONARY_DUTY_CYCLE, STARDARD_MOVEMENT_SPEED);
            Direction_1 = FORWARD;
            break;
        case RIGHT:
            setMotorSpeeds(STARDARD_MOVEMENT_SPEED, STATIONARY_DUTY_CYCLE);
            Direction_2 = FORWARD;
            break;
        }
        wait(0.1);
    }
    void turnaround(Turn t = LEFT)//Turns with both wheels in opposite directions
    {
        setMotorSpeeds(STARDARD_TURNING_SPEED, STARDARD_TURNING_SPEED);
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
        wait(0.1);
    }
};//End of class definition
