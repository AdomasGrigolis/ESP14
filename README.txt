Yo, here is some info to make sure everything is working as smoothly as possible
Before you begin ANY work, MAKE SURE TO UPDATE ALL FILES and ask people who
are potentially working with them if perhaps they have not pushed their updates

Files to include for the full program:
#include "mbed.h"
#include "C12832.h"//LCD library
#include <encoder.cpp>//Encoder class
#include <PWMdrive.cpp>//Motor driver
#include <pot.cpp>//Potentiometer
#include <sensor.cpp>//Line sensor

Objects of these classes:
Encoder:
TickingEncoder* wheel_left = new TickingEncoder(ENC_2_A_PIN, ENC_2_B_PIN);
TickingEncoder* wheel_right = new TickingEncoder(ENC_1_A_PIN, ENC_1_B_PIN);
Potentiometer:
Potentiometer* leftPot = new SamplingPotentiometer(A0, 3.3, 100.0);
Potentiometer* rightPot = new SamplingPotentiometer(A1, 3.3, 100.0);
Car:
Car car;
Line sensors (SENSOR):
SENSOR sen1(PC_4); //grey
SENSOR sen2(PC_0);//blue
SENSOR sen3(PC_5);//green
SENSOR sen4(PC_2);//red
SENSOR sen5(PC_3);//purple
SENSOR sen6(PB_0);//orange
LCD:
C12832 lcd(D11, D13, D12, D7, D10);

Imports:
QEI - Aaron Berk
C18232

Very important about encoder class:
When importing the QEI library, in QEI.h change Encoding encoding = X2_ENCODING to X4_ENCODING in the constructor
Otherwise, your measured values will be incorrect.
You can check if the encoders are working by using enc_test.cpp file as your main.

If you want the battery monitor:
Import through link:
http://os.mbed.com/users/EmbeddedSam/code/Nucleo_F401RE_DS271_Battery_Monitor/
Reffer to: battery.cpp
