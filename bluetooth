#include <mbed.h>
#include <C12832.h>

Serial hm10(PA_11, PA_12); //UART6 TX,RX
//RGB LED
DigitalOut redLED(D5);
DigitalOut greenLED(D9);
DigitalOut blueLED(D8);
//lcd
C12832 lcd(D11, D13, D12, D7, D10);
uint8_t my_command_length = 5;
uint8_t i;
char hm10_receive_buffer[5];

//class of RGB_LED
class RGB_LED                                           //Begin LED class definition
{

protected:                                          //Protected (Private) data member declaration
    DigitalOut red,green,blue;                      //Declaration of DigitalOut object
public:                                             //Public declarations
    RGB_LED(PinName r,PinName g,PinName b) : red(r),green(g),blue(b) {} //Constructor - user provided pin name is assigned to the DigitalOut
    //memeber function
    void RED (void)
    {
        redLED = 0;
        greenLED = 1;
        blueLED = 1;
    }
    void GREEN (void)
    {
        redLED = 1;
        greenLED = 0;
        blueLED = 1;
    }
    void YELLOW (void)
    {
        redLED = 0;
        greenLED = 0;
        blueLED = 1;
    }
};
RGB_LED led(D5,D9,D8);
int main()
{
    hm10.baud(9600);

    while(1) {

        //Receive command of specific length
        if(hm10.readable()) {
            while(i < my_command_length) {
                hm10_receive_buffer[i++] = hm10.getc();
            }
            i = 0;
            if(strcmp(hm10_receive_buffer, "REDON") == 0) {
                //LED RED COMMAND RECEIVED
                led.RED();
                memset(hm10_receive_buffer, 0, 5);
                // clear the command buffer after we have parsed the command
            } else if(strcmp(hm10_receive_buffer, "GREEN") == 0) {
                //LED GREEN COMMAND RECEIVED
                led.GREEN();
                memset(hm10_receive_buffer, 0, 5);
                // clear the command buffer after we have parsed the command
            } else if(strcmp(hm10_receive_buffer, "YELLOW") == 0) {
                //LED YELLOW COMMAND DOESN'T WORK BECAUSE WRONG LENGTH OF COMMAND
                led.YELLOW();
                memset(hm10_receive_buffer, 0, 5);
                // clear the command buffer after we have parsed the command
            }
        }
    }
}
