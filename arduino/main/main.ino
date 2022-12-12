// #include <Serial_CAN_Module.h>

// #include <SoftwareSerial.h>
// #include "bluetooth_config.h"
// #include "can_config.h"

// // DEFINES

// /*
//   Wiring HC05 <---> Arduino UNO
//      VCC <–> +5V
//      GND <–> GND
//      TXD <–> Pin 2 (RX)
//      RXD <–> Pin 3 (TX) via a voltage divider bridge from 5V to 3.3V
// */

// #define BLUETOOTH_RX 2
// #define BLUETOOTH_TX 3

// /*
//   Wiring OBD-II <---> Arduino UNO
//      VCC <–> +5V
//      GND <–> GND
//      TXD <–> Pin 4 (TX)
//      RXD <–> Pin 5 (RX)
// */

// #define CAN_RX 4
// #define CAN_TX 5

// // VARIABLES
// SoftwareSerial SerialBluetooth(BLUETOOTH_RX, BLUETOOTH_TX);

// Serial_CAN can;

// // PROTOTYPES
// bool init_AT();
// bool send_AT_cmd(const char * at_cmd);

// bool init_CAN();
// int convert_from_bytes_to_unite(int const id_pid, int const values1, int const values2);
// void sendPid(unsigned char __pid);
// bool getPid(unsigned char const __pid, int *value, int const nbr_of_bytes);



// // FONCTIONS
// /* BLUETOOTH */

// bool send_AT_cmd(const char * at_cmd) {
//   String _recv = "";
//   const char * recv;
  
//   Serial.print(at_cmd);
//   SerialBluetooth.write(at_cmd);
  
//   _recv = SerialBluetooth.readString();
//   _recv.trim();
  
//   recv = _recv.c_str();
//   const int len = _recv.length();

//   if(recv[len - 2] != 'O' || recv[len - 1] != 'K')
//     return false;
//   return true;
// }

// bool init_AT() {
//   Serial.print("-- AT config init --\n");
//   String recv = "";
//   uint8_t count_timeout = 0;
//   bool isOK = false;
  
//   while(isOK == false && count_timeout <= AT_INIT_TIMEOUT) {
//     isOK = send_AT_cmd(AT);
//     count_timeout++;
//   }

//   if(isOK) {
//     isOK &= send_AT_cmd(AT_VERSION);
//     isOK &= send_AT_cmd(AT_NAME);
//     isOK &= send_AT_cmd(AT_ROLE);
//     isOK &= send_AT_cmd(AT_RESET);
//   }

//   if(!isOK)
//     Serial.println("Errors occured.");

//   Serial.print("-- AT config finished --");
//   return isOK;
// }

// /* OBD-II */

// bool init_CAN() {
//   can.begin(CAN_TX, CAN_RX, 38400);
//   Serial.println("CAN transmission ok to start");
//   return true;
// }

// /* MAIN */

// // void setup()
// {
//   Serial.begin(115200);
//   SerialBluetooth.begin(38400);

//   Serial.print("Sketch:   ");   Serial.println(__FILE__);
//   Serial.print("Uploaded: ");   Serial.println(__DATE__);
//   Serial.println("READY");

//   init_AT();
//   init_CAN();
// }
// void loop()
// {
//   if (SerialBluetooth.available()) {
//     Serial.write(SerialBluetooth.read());
//   }
//   if (Serial.available()) {
//     SerialBluetooth.write(Serial.read());
//   }
// }
