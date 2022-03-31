#include "mbed.h"

#define THRESHOLD 0.5f

class SENSOR{
    private:
        AnalogIn inputSignal;
    public:
        SENSOR(PinName pin) : inputSignal(pin){;}
        bool sensorState(void){
            float voltage = inputSignal.read() *5;
            if (voltage >= THRESHOLD){return true;}
            else{return false;}
        } 
};//End of class definition
