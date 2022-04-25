#pragma once
#include <mbed.h>
#include <PWMdrive.cpp>


class BLUETOOTH
{
private:
    Serial hm10; //UART6 TX,RX
    Car car;
    uint8_t i;
    char hm10_receive_buffer[5];
public:
    BLUETOOTH(PinName USBTX, PinName USBRX): hm10(USBTX,USBRX)
    {
        hm10.baud(9600);
    }

    void control()
    {
        if(hm10.readable()) {
            while(i < 5) {
                hm10_receive_buffer[i++] = hm10.getc();
            }
            i = 0;
            if(strcmp(hm10_receive_buffer, "ROUND") == 0) {
                car.turnaround(LEFT);
                memset(hm10_receive_buffer, 0, 5);
                // clear the command buffer after we have parsed the command
            } else if(strcmp(hm10_receive_buffer, "SLOPE") == 0) {
                memset(hm10_receive_buffer, 0, 5);
                // clear the command buffer after we have parsed the command
            } else if(strcmp(hm10_receive_buffer, "TLEFT") == 0) {
                car.turnOneWheel(LEFT);
                memset(hm10_receive_buffer, 0, 5);
                // clear the command buffer after we have parsed the command
            } else if(strcmp(hm10_receive_buffer, "RIGHT") == 0) {
                car.turnOneWheel(RIGHT);
                memset(hm10_receive_buffer, 0, 5);
            }
        }
    }

};
