
#define BUGGY_WIDTH 195//distance between two wheel centres in mm
#define Pi 3.1415f

const float TURNING_CIRCLE = Pi*BUGGY_WIDTH*2.0f;
const float half_circle = TURNING_CIRCLE/2.00f;
//Bluetooth
InterruptIn bluetooth_receive(PA_12);

class BLUETOOTH
{
private:
    Serial hm10; //UART6 TX,RX
    uint8_t i;
    char hm10_receive_buffer[5];
public:
    BLUETOOTH(PinName USBTX, PinName USBRX): hm10(USBTX,USBRX)
    {
        hm10.baud(9600);
    }
    void control()
    {
        wheel_left->zero_dx();
        if(hm10.readable()) {
            while(i < 5) { hm10_receive_buffer[i++] = hm10.getc();}
            i = 0;
            if(strcmp(hm10_receive_buffer, "ROUND") == 0) {
                while (wheel_left->get_dx() > (half_circle)) {
                    //while (sen3.sensorState() == false) {
                    //lcd.locate(0,0);
                    //lcd.printf("normal");//}
                    car.turnaround(LEFT);
                    memset(hm10_receive_buffer, 0, 5);
                }

                
               

            } else if(strcmp(hm10_receive_buffer, "PAUSE") == 0) {
                car.stop();//}
                memset(hm10_receive_buffer, 0, 5);
            }

        }

    }
};


BLUETOOTH BT(PA_11,PA_12);



bluetooth_receive.rise(&BT, &BLUETOOTH::control);
