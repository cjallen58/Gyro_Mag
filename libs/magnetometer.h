#ifndef _MAGNETOMETER
#define _MAGNETOMETER


#define MAG_REG 0X0d

// Magnetometer registers
#define CONFIG_A 0x00
#define CONFIG_B 0x01
#define MODE_REG 0X02
#define MAG_READINGS 0X03
#define STATUS_REG 0X09
#define ID_REG 0X0a

// Magnetometer register configuratons
#define CRA_CONFIG 0b01010000
#define CRB_CONFIG 0b10000000
#define MODE_CONFIG 0b00000001

// Magnetometer resoution
#define MAG_RES 2.27


#endif