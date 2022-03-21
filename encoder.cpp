#pragma once
//When importing the QEI library, in QEI.h change Encoding encoding = X2_ENCODING to X4_ENCODING in the constructor
//Use these object in the main code
//TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
//TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
//To get speed for e.g. use wheel_right->get_speed();
#include <QEI.h>
#include <PWMdrive.cpp>

#define WHEEL_CIRCUMFERENCE 251.33f//In mm
#define ENCODER_MEASURE_PERIOD 0.1//Encoder pulses are checked at this periodicity
#define NO_PULS_PER_REV 1024//Pulses per revolution of enc disk (256*4)
#define GAP_LENGTH 10.0f//In mm
#define STOPPED_SPEED 20.0//In mm/s
#define PI 3.1415926f
//Pins
#define ENC_1_A_PIN PB_2
#define ENC_1_B_PIN PB_15
#define ENC_2_A_PIN PB_13
#define ENC_2_B_PIN PB_14

class TickingEncoder : public QEI
{
private:
    Ticker tick_enc;
    Car car;
    volatile int pulses;
    volatile double dx;
    volatile double speed;//mm/s
    volatile double ang_speed;//rad/s
    volatile bool no_line_var;
    volatile bool stopped;
    void enc_isr()
    {
        //Called to fetch encoder data by tick_enc
        pulses = getPulses();//Stores number of pulses
        speed = (((double)pulses/NO_PULS_PER_REV)*(WHEEL_CIRCUMFERENCE))/ENCODER_MEASURE_PERIOD;
        ang_speed = (((double)pulses/NO_PULS_PER_REV)*2*PI)/ENCODER_MEASURE_PERIOD;
        if (pos_dirr()*speed <= STOPPED_SPEED)stopped = true;
        else stopped = false;
        //Detection distance in case of line break
        if (no_line_var == true && stopped == false) {
            dx += ((double)pulses/NO_PULS_PER_REV)*(WHEEL_CIRCUMFERENCE);//In mm
            if (dx >= GAP_LENGTH) {
                car.stop();
                dx = 0;
            }
        }
        reset();//Resets the count
    }
public:
    TickingEncoder(PinName channelA, PinName channelB):QEI(channelA, channelB, NC, NO_PULS_PER_REV)
    {
//        this.car = car;
        dx = 0.0;
        no_line_var = false;
        stopped = false;
        reset();
        tick_enc.attach(callback(this, &TickingEncoder::enc_isr), ENCODER_MEASURE_PERIOD);
    }
    //Selectors
    double get_speed(){return speed;}
    double get_ang_speed(){return ang_speed;}
    int pos_dirr()//1 for positive dirr, 0 for none, and -1 for negative
    {
        if(speed > 0.0)return 1;
        else if (speed == 0.0) return 0;
        else return -1;
    }
/* Distance not used anymore as it's redundant with feedback controls
    double get_dx()
    {
        return dx;
    }
*/
    //Modifiers
    void zero_dx(){dx = 0.0;}
    void no_line(){no_line_var = true;}
    void line()
    {
        no_line_var = false;
        dx = 0.0;
    }
};
