#include "mbed.h"
#include "C12832.h"
#include <encoder.cpp>

C12832 lcd(D11, D13, D12, D7, D10);

TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);

int main()
{
    lcd.cls();
    lcd.set_auto_up(1);//Makes LCD not flicker by updating only when necessary
    while(1) {
        lcd.locate(0,0);
        lcd.printf("RSpeed: %0.3lf", wheel_right->get_speed());
        lcd.locate(0,10);
        lcd.printf("LSpeed: %0.3lf", wheel_left->get_speed());
    }
}
