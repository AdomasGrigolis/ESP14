#include "mbed.h"
#include "ds2781.h"
#include "OneWire_Methods.h"
#include "C12832.h"

C12832 lcd(D11, D13, D12, D7, D10);
DigitalInOut one_wire_pin(PC_12);
int VoltageReading, CurrentReading;
float Voltage, Current;
int main(){
while(1){
    VoltageReading = ReadVoltage();
    //Voltage = VoltageReading*0.00967;
    Voltage = VoltageReading*0.011;
    CurrentReading = ReadCurrent();
    Current = CurrentReading/6400.0;
    lcd.locate(0,0);              //Locate at (0,0)
    lcd.printf("Voltage= %.2f V",Voltage); 
}}