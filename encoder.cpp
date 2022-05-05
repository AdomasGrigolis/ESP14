#pragma once
//When importing the QEI library, in QEI.h change Encoding encoding = X2_ENCODING to X4_ENCODING in the constructor
//Use these object in the main code
//TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
//TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
//To get speed for e.g. use wheel_right->get_speed();
#include <QEI.h>

#define WHEEL_CIRCUMFERENCE 251.33f//In mm
#define ENCODER_MEASURE_PERIOD 0.001//Encoder pulses are checked at this periodicity
#define NO_PULS_PER_REV 1024//Pulses per revolution of enc disk (256*4)
#define GAP_LENGTH 10.0f//mm
#define STOPPED_SPEED 0.0//mm/s
#define PI 3.1415926f
#define QUARTER_CIRCLE 270.0f//mm
//Pins
#define ENC_1_A_PIN PB_2
#define ENC_1_B_PIN PB_15
#define ENC_2_A_PIN PB_13
#define ENC_2_B_PIN PB_14

class TickingEncoder : public QEI
{
private:
    Ticker tick_enc;
    volatile int pulses;
    double dx;
    volatile double speed;//mm/s
    volatile double ang_speed;//rad/s
    volatile bool no_line_var;//Flag to indicate that there is no line
    //This flag tells main to stop the motors/that buggy has turned 180 deg
    volatile bool flag;
    volatile bool count_q;//Quarter turn flag
    void enc_isr()
    {
        //Called to fetch encoder data by tick_enc
        pulses = getPulses();//Stores number of pulses
        speed = (((double)pulses/NO_PULS_PER_REV)*(WHEEL_CIRCUMFERENCE))/ENCODER_MEASURE_PERIOD;
        ang_speed = (((double)pulses/NO_PULS_PER_REV)*2*PI)/ENCODER_MEASURE_PERIOD;
        
        //Quarter circle counter
        if(count_q == true){
            dx += ((double)pulses/NO_PULS_PER_REV)*(WHEEL_CIRCUMFERENCE);//In mm
            if(dx >= QUARTER_CIRCLE){
                flag = true;
                zero_dx();
                count_q = false;
                }
        }
        //Detection distance in case of line break
        else if (no_line_var == true) {
            dx += ((double)pulses/NO_PULS_PER_REV)*(WHEEL_CIRCUMFERENCE);//In mm
            if (dx >= GAP_LENGTH) {
                flag = true;
                zero_dx();
            }
        }
        else zero_dx();//experimental
        reset();//Resets the count
    }
    void zero_dx(){dx = 0.0;}
public:
    TickingEncoder(PinName channelA, PinName channelB):QEI(channelA, channelB, NC, NO_PULS_PER_REV)
    {
        dx = 0.0;
        count_q = false;
        no_line_var = false;
        flag = false;
        reset();
        tick_enc.attach(callback(this, &TickingEncoder::enc_isr), ENCODER_MEASURE_PERIOD);
    }
    //Selectors
//    double get_speed(){return speed;}
    double get_ang_speed(){return ang_speed;}
    bool get_flag(){
        bool flag_old = flag;
        flag = false;
        return flag_old;
    }
    int pos_dirr()//1 for positive dirr, 0 for stationary, and -1 for negative
    {
        if(ang_speed > 0.0)return 1;
        else if (ang_speed == 0.0) return 0;
        else return -1;
    }
//    double get_dx(){return dx;}
    //Modifiers
    void no_line(){no_line_var = true;}
    void line()
    {
        no_line_var = false;
        zero_dx();
    }
    void quarter_circle(){count_q = true;}
};
