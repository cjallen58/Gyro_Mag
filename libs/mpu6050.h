#ifndef _MPU6050
#define _MPU6050

#define MPU6050_ADDR 0x68

// MPU6050 registers
#define MPU6050_PWR_CTRL_1 0x6B
#define MPU6050_GYRO_CONFIG 0X1B
#define MPU6050_ACCEL_CONFIG 0X1C
#define MPU6050_GYRO_READINGS 0x43
#define MPU6050_ACCEL_READINGS 0x3B
#define MPU6050_TEMP_READING 0x41
#define MPU6050_WHOAMI_REG 0x75

// Gyro and accelerometer configurations
#define MPU6050_GYRO_250DPS 0b00000000
#define MPU6050_GYRO_500DPS 0b00001000
#define MPU6050_GYRO_1000DPS 0b00010000
#define MPU6050_GYRO_2000DPS 0b00011000

#define MPU6050_ACCEL_2G 0b00000000
#define MPU6050_ACCEL_4G 0b00001000
#define MPU6050_ACCEL_8G 0b00010000
#define MPU6050_ACCEL_16G 0b00011000

// Gyro and accelerometer scale factors
#define GYRO_250DPS_FACTOR 131.0
#define GYRO_500DPS_FACTOR 65.5
#define GYRO_1000DPS_FACTOR 32.8
#define GYRO_2000DPS_FACTOR 16.4

#define ACCEL_2G_FACTOR 16384.0
#define ACCEL_4G_FACTOR 8192.0
#define ACCEL_8G_FACTOR 4096.0
#define ACCEL_16G_FACTOR 2048.0


// Power control values
#define MPU6050_PWR_CTRL_RESET 0b10000000
#define MPU6050_PWR_CTRL_WAKE 0b00000000

/*############ Function Declarations ############*/

static void mpu6050_reset();

static void mpu6050_config(uint8_t gyro_config, uint8_t accel_config);

static void mpu6050_data_read(int16_t gyro[3], int16_t accel[3], int16_t *temp);

static void mpu6050_calibrate(int32_t gyro_cal[3], int32_t accel_cal[3]);



#endif