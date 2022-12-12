
#define STANDARD_CAN_11BIT      1       // That depands on your car. some 1 some 0. 

/* CAN ID PID */
#if STANDARD_CAN_11BIT
#define CAN_ID_PID          0x7DF
#else
#define CAN_ID_PID          0x18db33f1
#endif

/* filts and masks */
#if STANDARD_CAN_11BIT
unsigned long mask[4] = 
{
    0, 0x7FC,                // ext, maks 0
    0, 0x7FC,                // ext, mask 1
};
unsigned long filt[12] = 
{
    0, 0x7E8,                // ext, filt 0
    0, 0x7E8,                // ext, filt 1
    0, 0x7E8,                // ext, filt 2
    0, 0x7E8,                // ext, filt 3
    0, 0x7E8,                // ext, filt 4
    0, 0x7E8,                // ext, filt 5
};
#else
unsigned long mask[4] =
{
    1, 0x1fffffff,               // ext, maks 0
    1, 0x1fffffff,
};
unsigned long filt[12] =
{
    1, 0x18DAF110,                // ext, filt
    1, 0x18DAF110,                // ext, filt 1
    1, 0x18DAF110,                // ext, filt 2
    1, 0x18DAF110,                // ext, filt 3
    1, 0x18DAF110,                // ext, filt 4
    1, 0x18DAF110,                // ext, filt 5
};
#endif

/*  PIDs adresses
    /!\ if you want to add a PID, you have to increase the MAX_PID and handle the conversion
    Careful, the PID of the list of PID supported isn't count in MAX_PID
    for doc : https://en.wikipedia.org/wiki/OBD-II_PIDs#Service_01_PID_03
    */

#define PID_SUPPORTED_1_20      0x00
// 1 --> 20
#define PID_ENGINE_LOAD         0x04
#define PID_COOLANT_TEMP        0x05
#define PID_FUEL_PRESSURE       0x0A
#define PID_FUEL_ABS_PRESSURE   0x0B
#define PID_ENGINE_PRM          0x0C
#define PID_VEHICLE_SPEED       0x0D
#define PID_TEMP_AIR_INTAKE     0x0F
#define PID_AIR_FLOW_RATE       0x10
#define PID_THROTTLE_POS        0x11

#define PID_SUPPORTED_21_40     0x20
// 21 --> 40
#define PID_FUEL_REL_PRESSURE   0x22
#define PID_FUEL_ABS_PRESSURE   0x23
#define PID_FUEL_TANK_LVL       0x2F

#define PID_SUPPORTED_41_60     0x40
// 41--> 60
#define PID_ABS_ENGINE_LOAD     0x43
#define PID_AIR_FUEL_RATIO      0x44
#define PID_REL_THROTTLE_POS    0x45
#define PID_TEMP_AIR_AMBIENT    0x46
#define PID_FUEL_TYPE           0x51
#define PID_ETHANOL_FUEL        0x52
#define PID_REL_ACC_PEDAL_POS   0x5A
#define PID_OIL_TEMP            0x5C
#define PID_ENGINE_FUEL_RATE    0x5E

#define PID_SUPPORTED_61_80     0x60
// 61 --> 80
#define PID_DEMANDED_TORQUE     0x61
#define PID_ACTUAL_TORQUE       0x62
#define PID_REF_TORQUE          0x63

/*  PIDs conversion, with units
    for doc : https://en.wikipedia.org/wiki/OBD-II_PIDs#Service_01_PID_03
    */

#define PID_ENGINE_LOAD_CONVERSION(load)                    (load * 100) / 256                  // %
#define PID_COOLANT_TEMP_CONVERSION(temp)                   (temp - 40)                         // °C
#define PID_FUEL_PRESSURE_CONVERSION(press)                 (3 * press)                         // kPa
#define PID_FUEL_ABS_PRESSURE_CONVERSION(press)             (press)                             // kPa
#define PID_ENGINE_PRM_CONVERSION(rpm1, rpm2)               (rpm1 * 256 + rpm2) / 4             // rpm
#define PID_VEHICLE_SPEED_CONVERSION(speed)                 (speed)                             // km/h
#define PID_TEMP_AIR_INTAKE_CONVERSION(temp)                (temp - 40)                         // °C
#define PID_AIR_FLOW_RATE_CONVERSION(rate1, rate2)          (256 * rate1 + rate2) / 100         // g/s
#define PID_THROTTLE_POS_CONVERSION(throttle)               (throttle) * 100 / 256              // %

#define PID_FUEL_REL_PRESSURE_CONVERSION(press1, press2)    (256 * press1 + press2) * 0.079     // kPa
#define PID_FUEL_ABS_PRESSURE_CONVERSION(press1, press2)    (256 * press1 + press2) * 10        // kPa
#define PID_FUEL_TANK_LVL_CONVERSION(lvl)                   (lvl * 100) / 256                   // %

#define PID_ABS_ENGINE_LOAD_CONVERSION(load1, load2)        (256 * load1 + load2) * 100 / 256   // %
#define PID_AIR_FUEL_RATIO_CONVERSION(ratio1, ratio2)       (256 * ratio1 + ratio2) * 2 / 65536 // ratio
#define PID_REL_THROTTLE_POS_CONVERSION(pos)                (pos * 100) / 256                   // %
#define PID_TEMP_AIR_AMBIENT_CONVERSION(temp)               (temp - 40)                         // °C
#define PID_FUEL_TYPE_CONVERSION(type)                      (type)                              // See doc : https://en.wikipedia.org/wiki/OBD-II_PIDs#Fuel_Type_Coding
#define PID_ETHANOL_FUEL_CONVERSION(fuel)                   (fuel * 100) / 256                  // %
#define PID_REL_ACC_PEDAL_POS_CONVERSION(pos)               (pos * 100) / 256                   // %
#define PID_OIL_TEMP_CONVERSION(temp)                       (temp - 40)                         // °C
#define PID_ENGINE_FUEL_RATE_CONVERSION(rate1, rate2)       (256 * rate1 + rate2) / 20          // L/h

#define PID_DEMANDED_TORQUE_CONVERSION(torque)              (torque - 125)                      // %
#define PID_ACTUAL_TORQUE_CONVERSION(torque)                (torque - 125)                      // %
#define PID_REF_TORQUE_CONVERSION(torque1, torque2)         (256 * torque1 + torque2)           // Nm

/* PIDs tab */

#define MAX_PID 24
int pids[MAX_PID] = {
    PID_ENGINE_LOAD,
    PID_COOLANT_TEMP,
    PID_FUEL_PRESSURE,
    PID_FUEL_ABS_PRESSURE,
    PID_ENGINE_PRM,
    PID_VEHICLE_SPEED,
    PID_TEMP_AIR_INTAKE,
    PID_AIR_FLOW_RATE,
    PID_THROTTLE_POS,

    PID_FUEL_REL_PRESSURE,
    PID_FUEL_ABS_PRESSURE,
    PID_FUEL_TANK_LVL,

    PID_ABS_ENGINE_LOAD,
    PID_AIR_FUEL_RATIO,
    PID_REL_THROTTLE_POS ,
    PID_TEMP_AIR_AMBIENT,
    PID_FUEL_TYPE,
    PID_ETHANOL_FUEL,
    PID_REL_ACC_PEDAL_POS,
    PID_OIL_TEMP ,
    PID_ENGINE_FUEL_RATE,

    PID_DEMANDED_TORQUE,
    PID_ACTUAL_TORQUE,
    PID_REF_TORQUE
};

#define MAX_PID_SUPPORTED 4
int pids_supp[MAX_PID_SUPPORTED] = {
    PID_SUPPORTED_1_20,
    PID_SUPPORTED_21_40,
    PID_SUPPORTED_41_60,
    PID_SUPPORTED_61_80
}