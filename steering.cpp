#include "mbed.h"
#include <QEI.h>

const float ENCODER_MEASURE_PERIOD = 1.0f;//Encoder pulses are checked every 1 second
const int NO_PULS_PER_REV = 1024;//Pulses per revolution (256*4)
const float WHEEL_CIRCUMFERENCE = 276.46f;//In milimetres
const float DISTANCE_between_WHEELS = // In milimetres 
const float TURNING_CIRCLE = 
volatile double R_dx = 0;//Wheel linear distance driven mm
volatile double L_dx = 0;
volatile double R_speed = 0;//Wheel linear speed (constant period) mm/s
volatile double L_speed = 0;

//Speaker class
class Speaker {
    private:
    DigitalOut outputSignal;
    bool state; // Can be set to either 1 or 0 to record output value
    public:
    Speaker(PinName pin) : outputSignal(pin){off();}
    void on(void){//Sets the pin high
        outputSignal = 1;
        state = true;
    }
    void off(void){//Sets the pin low
        outputSignal = 0;
        state = false;
        }
    void toggle(void){//Sets switches the pin's state
        if(state)off();
        else on();
    }
    bool getState(void)//Allows to access state of the sgianl
    {
        return state;
    }
};


//Simply the type of encoding in enumerator form (do not change enum values)
//enum encoding {X2_ENCODING, X4_ENCODING};
//encoding enc = X4_ENCODING;
//Right and left wheel encoder bject definition
QEI wheel_right(D8, D9, NC, NO_PULS_PER_REV);
QEI wheel_left(D10, D11, NC, NO_PULS_PER_REV);

Ticker tick_enc;//Fetches number of pulses every so period
void enc_isr(){//Called to fetch encoder data by tick_enc
    int R_pulses = wheel_right.getPulses();//Stores number of pulses
    int L_pulses = wheel_left.getPulses();
    wheel_right.reset();//Resets the count
    wheel_left.reset();
    //Further commands could be moved into FSM to preserve performance
    R_speed = R_pulses/(44*WHEEL_CIRCUMFERENCE)*ENCODER_MEASURE_PERIOD;
    L_speed = L_pulses/(44*WHEEL_CIRCUMFERENCE)*ENCODER_MEASURE_PERIOD;//Forumla from library file QEI. 44 as X4 encoding type
    R_dx = R_dx + R_speed/ENCODER_MEASURE_PERIOD;//Old distance + new_speed*time=new_distance
    L_dx = L_dx + L_speed/ENCODER_MEASURE_PERIOD;
}

int main(void){
//    int R_revol = 0;//Keeps track of how many revolutions complete
    tick_enc.attach(&enc_isr, ENCODER_MEASURE_PERIOD);//Checks rotation every one second
    //Speaker for debugging purposes
    Ticker spkr_tick;
    Speaker spkr(D6);
    while(true){
            if (R_speed > 100){
                spkr_tick.attach(&spkr, &Speaker::toggle,1.0/1400.0);
                }
            else spkr_tick.detach();
            wait(0.2);
        }
    while(1)
    {   double distance_line =R_dx;
        while(curr_pos - start_position < 500)
            {  
            curr_pos=(R_dx+L_dx)/2; 
            // setting same pwm to left wheel and right wheel
            }
        --//setting 0 volocity to right wheel, left wheel stay unchanged
        double distance_circle = L_dx;
        while(L_dx - distance_circle < 434.26)
            {
             // setting 0 volocity to right wheel, left wheel stay unchanged    
            }
    }
    typedef enum {driving_initialisation, driving, stop, turning_initialisation, turning} BuggyState;
    BuggyState state = initialisation;
    
    while (1)
    {
        switch (state)
        {
            case driving_initialisation:
            {
                R_dx = 0;
                L_dx = 0;
                state = driving;
                break;
            }
            case driving:
            {   //set PWM
                curr_pos=(R_dx+L_dx)/2;
                if(curr_pos> 500)
                    state = stop;
                break;    
            }
            case stop:
            {   
                //set duty cycle to 0.5
                state = turning_initialisation;
                break;    
            }
            case turning_initialisation:
            {
                L_dx=0;
                R_dx=0;
                break;    
            }
            case turning:
            {   //set PWM
                if(L_dx > TURNING_CIRCLE)
                    state = driving_initialisation;
                break; 
            }
            default:
                break;
    }