//When importing the QEI library, in QEI.h change Encoding encoding = X2_ENCODING to X4_ENCODING in the constructor
//Use these object in the main code
//TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
//TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
//To get speed for e.g. use wheel_right->get_speed();
#include <QEI.h>

#define WHEEL_CIRCUMFERENCE 251.33f//In mm
#define ENCODER_MEASURE_PERIOD 0.1f//Encoder pulses are checked every 1 second
#define NO_PULS_PER_REV 1024//Pulses per revolution (256*4)
//Pins
#define ENC_1_A_PIN PB_2
#define ENC_1_B_PIN PB_15
#define ENC_2_A_PIN PB_13
#define ENC_2_B_PIN PB_14

class TickingEncoder : public QEI {
    private:
        Ticker tick_enc;
        volatile int pulses;
        volatile double dx;
        volatile double speed;
        void enc_isr()
        {//Called to fetch encoder data by tick_enc
            pulses = getPulses();//Stores number of pulses
            dx += ((double)pulses/NO_PULS_PER_REV)*(WHEEL_CIRCUMFERENCE);//In mm
            speed = (((double)pulses/NO_PULS_PER_REV)*(WHEEL_CIRCUMFERENCE))/ENCODER_MEASURE_PERIOD;
            reset();//Resets the count
        }
    public:
        TickingEncoder(PinName channelA, PinName channelB):QEI(channelA, channelB, NC, NO_PULS_PER_REV)
        {
            dx = 0;
            reset();
            tick_enc.attach(callback(this, &TickingEncoder::enc_isr), ENCODER_MEASURE_PERIOD);
        }
        //Selectors
        double get_speed(){return speed;}
        double get_dx(){return dx;}
        //Modifiers
        void zero_dx(){dx = 0;}
};
