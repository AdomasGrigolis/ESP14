#include "mbed.h" //includes mbed library
#include "C12832.h" //includes LCD screen lib

// Max X value of LCD screen

// Max Y value of LCD screen


C12832 lcd(D11, D13, D12, D7, D10);     //Creates an LCD Object from the LCD library
DigitalOut led(PB_2);//yellow
float threshold = 0.5;
class SENSOR{
    private:
        AnalogIn inputSignal;
    public:
        SENSOR(PinName pin) : inputSignal(pin){;}
        
        bool sensorState(void){

            float voltage = inputSignal.read() *5;
            if (voltage >= threshold){
                return true;
            }
            else{
                return false;   
            }
        }
    
      
};
SENSOR sen1(PC_4); //grey
SENSOR sen2(PB_1);//blue
SENSOR sen3(PC_5);//green
SENSOR sen4(PC_2);//red
SENSOR sen5(PC_3);//purple
SENSOR sen6(PC_1);//orange Power drop on this pin because it is shared with the joystick.

float a;

int main()
{
    int sensorsOn;
    float distance;
    while(1) {
        led = 1;
        distance = 0.0;
        sensorsOn = 0;
        if (sen1.sensorState() == true){
            lcd.locate(20,0);
            lcd.printf("1: on");
            distance += 45;
            sensorsOn += 1;
        }
        else if (sen1.sensorState() == false){
            lcd.locate(20,0);
            lcd.printf("1: off");
            
        }
        if (sen2.sensorState() == true){
            lcd.locate(20,10);
            lcd.printf("2: on");
            distance += 25;
            sensorsOn += 1;
        }
        else if (sen2.sensorState() == false){
            lcd.locate(20,10);
            lcd.printf("2: off");
        }
        if (sen3.sensorState() == true){
            lcd.locate(20,20);
            lcd.printf("3: on");
            distance += 10;
            sensorsOn += 1;
        }
        else if (sen3.sensorState() == false){
            lcd.locate(20,20);
            lcd.printf("3: off");
        }
        if (sen4.sensorState() == true){
            lcd.locate(80,0);
            lcd.printf("4: on");
            distance -= 10;
            sensorsOn += 1;
        }
        else if (sen4.sensorState() == false){
            lcd.locate(80,0);
            lcd.printf("4: off");
        }
        if (sen5.sensorState() == true){
            lcd.locate(80,10);
            lcd.printf("5: on");
            distance -= 25;
            sensorsOn += 1;
        }
        else if (sen5.sensorState() == false){
            lcd.locate(80,10);
            lcd.printf("5: off");
        }
        if (sen6.sensorState() == true){
            lcd.locate(80,20);
            lcd.printf("6: on");
            distance -= 45;
            sensorsOn += 1;
        }
        else if (sen6.sensorState() == false){
            lcd.locate(80,20);
            lcd.printf("6: off");
            
        }
        distance = distance / sensorsOn;
        lcd.locate(60,0);
        lcd.printf("%.0f",distance);

        


    }

}
