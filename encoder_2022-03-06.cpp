//When importing the QEI library, in QEI.h change Encoding encoding = X2_ENCODING to X4_ENCODING in the constructor
//Use these object in the main code
//QEI* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
//QEI* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
#include <QEI.h>

#define WHEEL_CIRCUMFERENCE 0.27646//In meters
#define ENCODER_MEASURE_PERIOD 0.05//Encoder pulses are checked every 0.05 second
#define NO_PULS_PER_REV 1024//Pulses per revolution (256*4)
//Pins
#define ENC_1_A_PIN PB_2
#define ENC_1_B_PIN PB_15
#define ENC_2_A_PIN PB_13
#define ENC_2_B_PIN PB_14

#define gear_ratio 15

class TickingEncoder : public QEI {
    private:
        Ticker tick_enc;
        volatile int encoder_pulses, last_pulses, current_pulses,;
        volatile double dx, current_speed, total_distance;
		void enc_isr()
		{//Called to fetch encoder data by tick_enc
			encoder_pulses = encoder.getPulses();
			last_pulses = current_pulses;
			current_pulses = encoder_pulses;
			current_speed = (current_pulses - last_pulses) / 1024 * ENCODER_MEASURE_PERIOD * gear_ratio;
			total_distance += ENCODER_MEASURE_PERIOD * current_speed ;
        }
    public:
        TickingEncoder(PinName channelA, PinName channelB):QEI(channelA, channelB, NC, NO_PULS_PER_REV)
        {
            dx = 0;
            tick_enc.attach(callback(this, &TickingEncoder::enc_isr), ENCODER_MEASURE_PERIOD);
        }
        //Selectors
        double get_speed(){return current_speed;}
        double get_dx(){return total_distance;}
        //Modifiers
        void zero_dx(){dx=0;}
};
