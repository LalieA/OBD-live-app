#include <Serial_CAN_Module.h>

#include <SoftwareSerial.h>
#include "bluetooth_config.h"
#include "can_config.h"

// DEFINES

/*
  Wiring HC05 <---> Arduino UNO
     VCC <–> +5V
     GND <–> GND
     TXD <–> Pin 2 (RX)
     RXD <–> Pin 3 (TX) via a voltage divider bridge from 5V to 3.3V
*/

#define BLUETOOTH_RX 2
#define BLUETOOTH_TX 3

/*
  Wiring OBD-II <---> Arduino UNO
     VCC <–> +5V
     GND <–> GND
     TXD <–> Pin 4 (TX)
     RXD <–> Pin 5 (RX)
*/

#define CAN_RX 4
#define CAN_TX 5

// VARIABLES
SoftwareSerial SerialBluetooth(BLUETOOTH_RX, BLUETOOTH_TX);

Serial_CAN can;
int pids_supp_mask[MAX_PID];

// PROTOTYPES
bool init_AT();
bool send_AT_cmd(const char * at_cmd);

bool init_CAN();
bool init_PIDs();
bool is_pid_supported(int id_pid, int pids_supp, int offset) ;
int convert_from_bytes_to_unite(int const id_pid_index, int const values1, int const values2);
void sendPid(unsigned char __pid);
bool getPid(unsigned char const pid_id_index, int *value);



// FONCTIONS
/* BLUETOOTH */

bool send_AT_cmd(const char * at_cmd) {
  String _recv = "";
  const char * recv;
  
  Serial.print(at_cmd);
  SerialBluetooth.write(at_cmd);
  
  _recv = SerialBluetooth.readString();
  _recv.trim();
  
  recv = _recv.c_str();
  const int len = _recv.length();

  if(recv[len - 2] != 'O' || recv[len - 1] != 'K')
    return false;
  return true;
}

bool init_AT() {
  Serial.print("-- AT config init --\n");
  String recv = "";
  uint8_t count_timeout = 0;
  bool isOK = false;
  
  while(isOK == false && count_timeout <= AT_INIT_TIMEOUT) {
    isOK = send_AT_cmd(AT);
    count_timeout++;
  }

  if(isOK) {
    isOK &= send_AT_cmd(AT_VERSION);
    isOK &= send_AT_cmd(AT_NAME);
    isOK &= send_AT_cmd(AT_ROLE);
    isOK &= send_AT_cmd(AT_RESET);
  }

  if(!isOK)
    Serial.println("Errors occured.");

  Serial.print("-- AT config finished --");
  return isOK;
}

/* OBD-II */

bool init_CAN() {
  can.begin(CAN_TX, CAN_RX, 38400);      // tx, rx

  Serial.println(can.canRate(CAN_RATE_500) ? "set can rate ok" : "set can rate fail");
  Serial.println(can.setMask(mask) ? "set mask ok" : "set mask fail");
  Serial.println(can.setFilt(filt) ? "set filt ok" : "set filt fail");
  return true;
}

// TODO a tester 
bool init_PIDs() {
    int pids_supp_1_20;
    getPid(pids_supp[0], pids_supp_1_20);
    int pids_supp_21_40;
    getPid(pids_supp[1], pids_supp_21_40);
    int pids_supp_41_60;
    getPid(pids_supp[2], pids_supp_41_60);
    int pids_supp_61_80;
    getPid(pids_supp[3], pids_supp_61_80);

    for (int pid_id; pid_id < MAX_PID; pid_id++){
        if (pids[pid_id] <= 0x20)
        {
            pids_supp_mask[pid_id] = is_pid_supported(pid_id, pids_supp_1_20, 0x0);
        }
        else if (pids[pid_id] <= 0x40)
        {
            pids_supp_mask[pid_id] = is_pid_supported(pid_id, pids_supp_21_40, 0x20);
        }
        else if (pids[pid_id] <= 0x60)
        {
            pids_supp_mask[pid_id] = is_pid_supported(pid_id, pids_supp_41_60, 0x40);
        }
        else if (pids[pid_id] <= 0x80)
        {
            pids_supp_mask[pid_id] = is_pid_supported(pid_id, pids_supp_61_80, 0x60);
        }
        else
        {
            Serial.println("The id proposed in pid supported checking doesn't exist");
        }
    }
}

bool is_pid_supported(int id_pid, int pids_supp, int offset) {
    
    return (pids_supp >> (0x20 + offset - id_pid)) & 0x1;
}

void sendPid(unsigned char __pid) {
    unsigned char tmp[8] = {0x02, 0x01, __pid, 0, 0, 0, 0, 0};

#if STANDARD_CAN_11BIT
    can.send(CAN_ID_PID, 0, 0, 8, tmp);   // SEND TO ID:0X55
#else
    can.send(CAN_ID_PID, 1, 0, 8, tmp);   // SEND TO ID:0X55
#endif
}

bool getPid(unsigned char const pid_id_index, int *value)
{
    sendPid(pids[pid_id_index]);
    unsigned long __timeout = millis();

    while(millis() - __timeout < 3000)  // 1s time out
    {
        unsigned long id = 0;
        unsigned char buf[8];

        if (can.recv(&id, buf)) {  // check if get data
            if(buf[1] == 0x41)
            { 
                *value = convert_from_bytes_to_unite(pid_id_index, buf[3], buf[4]);       
                return 1;
            }
        }
    }
    Serial.println("timed out");    
    return 0;
}

int convert_from_bytes_to_unite(int const id_pid, int const values1, int const values2)
{
    switch (id_pid)
    {
    case 0: //PID_ENGINE_LOAD:
        return PID_ENGINE_LOAD_CONVERSION(values1);
    case 1: //PID_COOLANT_TEMP:
        return PID_COOLANT_TEMP_CONVERSION(values1);
    case 2: //PID_FUEL_PRESSURE:
        return PID_FUEL_PRESSURE_CONVERSION(values1);
    case 3: //PID_FUEL_ABS_PRESSURE:
        return PID_FUEL_ABS_PRESSURE_CONVERSION(values1, values2);
    case 4: //PID_ENGINE_PRM:
        return PID_ENGINE_PRM_CONVERSION(values1, values2);
    case 5: //PID_VEHICLE_SPEED:
        return PID_VEHICLE_SPEED_CONVERSION(values1);
    case 6: //PID_TEMP_AIR_INTAKE:
        return PID_TEMP_AIR_INTAKE_CONVERSION(values1);
    case 7: //PID_AIR_FLOW_RATE:
        return PID_AIR_FLOW_RATE_CONVERSION(values1, values2);
    case 8: //PID_THROTTLE_POS:
        return PID_THROTTLE_POS_CONVERSION(values1);

    case 9: //PID_FUEL_REL_PRESSURE:
        return PID_FUEL_REL_PRESSURE_CONVERSION(values1, values2);
    case 10: //PID_FUEL_ABS_PRESSURE:
        return PID_FUEL_ABS_PRESSURE_CONVERSION(values1, values2);
    case 11: //PID_FUEL_TANK_LVL:
        return PID_FUEL_TANK_LVL_CONVERSION(values1);

    case 12: //PID_ABS_ENGINE_LOAD:
        return PID_ABS_ENGINE_LOAD_CONVERSION(values1, values2);
    case 13: //PID_AIR_FUEL_RATIO:
        return PID_AIR_FUEL_RATIO_CONVERSION(values1, values2);
    case 14: //PID_REL_THROTTLE_POS:
        return PID_REL_THROTTLE_POS_CONVERSION(values1);
    case 15: //PID_TEMP_AIR_AMBIENT:
        return PID_TEMP_AIR_AMBIENT_CONVERSION(values1);
    case 16: //PID_FUEL_TYPE:
        return PID_FUEL_TYPE_CONVERSION(values1);
    case 17: //PID_ETHANOL_FUEL:
        return PID_ETHANOL_FUEL_CONVERSION(values1);
    case 18: //PID_REL_ACC_PEDAL_POS:
        return PID_REL_ACC_PEDAL_POS_CONVERSION(values1);
    case 19: //PID_OIL_TEMP:
        return PID_OIL_TEMP_CONVERSION(values1);
    case 20: //PID_ENGINE_FUEL_RATE:
        return PID_ENGINE_FUEL_RATE_CONVERSION(values1, values2);
    
    case 21: //PID_DEMANDED_TORQUE:
        return PID_DEMANDED_TORQUE_CONVERSION(values1);
    case 22: //PID_ACTUAL_TORQUE:
        return PID_ACTUAL_TORQUE_CONVERSION(values1);
    case 23: //PID_REF_TORQUE:
        return PID_REF_TORQUE_CONVERSION(values1, values2);
    default:
        Serial.println("Error : pid requested doesn't exist");
        break;
    }
    return 0;
}

/* MAIN */

void setup()
{
  Serial.begin(115200);
  SerialBluetooth.begin(38400);

  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println("READY");

  init_AT();
  init_CAN();
  init_PIDs();
  //TODO send on bluetooth pids_supp_mask.
}

void loop()
{
  //TODO send the values a cording to the 2 pids tabs (id and supp)
  if (SerialBluetooth.available()) {
    Serial.write(SerialBluetooth.read());
  }
  if (Serial.available()) {
    SerialBluetooth.write(Serial.read());
  }
}
