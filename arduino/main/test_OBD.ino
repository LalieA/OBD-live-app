#include <Serial_CAN_Module.h>
#include <SoftwareSerial.h>

#define STANDARD_CAN_11BIT      1       // That depands on your car. some 1 some 0. 

Serial_CAN can;

#define can_tx  2           // tx of serial can module connect to D2
#define can_rx  3           // rx of serial can module connect to D3

#define PID_ENGIN_PRM       0x0C
#define PID_VEHICLE_SPEED   0x0D
#define PID_COOLANT_TEMP    0x05

#if STANDARD_CAN_11BIT
#define CAN_ID_PID          0x7DF
#else
#define CAN_ID_PID          0x18db33f1
#endif


void sendPid(unsigned char __pid) {
    unsigned char tmp[8] = {0x02, 0x01, __pid, 0, 0, 0, 0, 0};

#if STANDARD_CAN_11BIT
    can.send(CAN_ID_PID, 0, 0, 8, tmp);   // SEND TO ID:0X55
#else
    can.send(CAN_ID_PID, 1, 0, 8, tmp);   // SEND TO ID:0X55
#endif
}

void recvPid() {
  unsigned long __timeout = millis();

    while(millis()-__timeout < 1000)      // 1s time out
    {
        unsigned long id  = 0;
        unsigned char buf[8];

        if (can.recv(&id, buf)) {                // check if get data

            if(buf[1] == 0x41)
            {
                *s = buf[3];
                return 1;
            }
        }
    }
}

bool getSpeed(int *s)
{
    sendPid(PID_VEHICLE_SPEED);
    unsigned long __timeout = millis();

    while(millis()-__timeout < 1000)      // 1s time out
    {
        unsigned long id  = 0;
        unsigned char buf[8];

        if (can.recv(&id, buf)) {                // check if get data

            if(buf[1] == 0x41)
            {
                *s = buf[3];
                return 1;
            }
        }
    }

    return 0;
}

void setup() {
    Serial.begin(115200);
    while(!Serial);   
    can.begin(can_tx, can_rx, 38400);
    Serial.println("-- TEST PID --");

    for(uint8_t pid = 0; pid < 129; pid++) {
      sendPid(pid);
      unsigned long id  = 0;
      unsigned char buf[8];

        if (can.recv(&id, buf))
    }
    delay(500);
}

void loop() {

    // int __speed = 0;
    // int ret = getSpeed(&__speed);
    // if(ret)
    // {
    //     Serial.print("Vehicle Speed: ");
    //     Serial.print(__speed);
    //     Serial.println(" kmh");
    // }
}