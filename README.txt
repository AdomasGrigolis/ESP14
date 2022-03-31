Yo, here is some info to make sure everything is working as smoothly as possible
Before you begin ANY work, MAKE SURE TO UPDATE ALL FILES and ask people who
are potentially working with them if perhaps they have not pushed their updates
Files to include for the full program:
#include "mbed.h"
#include "C12832.h"//LCD library
#include <encoder.cpp>//Encoder class
#include <PWMdrive.cpp>//Motor driver
#include <pot.cpp>//Potentiometer
#include <sensor.cpp>//Line sensor class

Very important about encoder class:
When importing the QEI library, in QEI.h change Encoding encoding = X2_ENCODING to X4_ENCODING in the constructor
Otherwise, your measured values will be incorrect
You can check if the encoder are working by using enc_test.cpp file as your main
