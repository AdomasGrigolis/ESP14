#include <QEI.h>

#define WHEEL_CIRCUMFERENCE 276.46f//In milimetres
#define ENCODER_MEASURE_PERIOD 1.0f//Encoder pulses are checked every 1 second
#define NO_PULS_PER_REV 1024//Pulses per revolution (256*4)

class TickingEncoder : public QEI {
    private:
        Ticker tick_enc;
        int pulses;
        double dx;
    public:
            void enc_isr(){//Called to fetch encoder data by tick_enc
                pulses = getPulses();//Stores number of pulses
                reset();//Resets the count
                dx = dx + pulses/(44*WHEEL_CIRCUMFERENCE);//Old distance + new_speed*time=new_distance
        }
        TickingEncoder(PinName channelA, PinName channelB, PinName index):QEI(channelA, channelB, index, NO_PULS_PER_REV)
        {
            tick_enc.attach(callback(this, &TickingEncoder::enc_isr), ENCODER_MEASURE_PERIOD);
        }
        //Selectors
        int get_speed(){return pulses/(44*WHEEL_CIRCUMFERENCE)*ENCODER_MEASURE_PERIOD;}
        int get_dx(){return dx;}
        //Modifiers
        void zero_dx(){dx=0;}
};
