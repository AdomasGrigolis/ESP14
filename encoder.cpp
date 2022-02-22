#include "mbed.h"
#include "QEI.h"

Serial pc(USBTX, USBRX);
QWI wheel_left(D8, D9, NC, 624);    
QEI wheel_right(D10, D11, NC, 624);

int main()
{   while(1)
    {   int count1, count2 =0;
        count1 = wheel_left.getPulses()
        waint(0.1);
        count2 = wheel_left.getPulses()
        float speed = (count2 - count1)/0.1; 
    }

}

