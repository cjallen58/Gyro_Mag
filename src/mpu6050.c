#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../libs/mpu6050.h"
#include "../libs/i2c_config.h"

static void mpu6050_reset() {

    uint8_t reg;
    uint8_t buf[] = {MPU6050_PWR_CTRL_1, MPU6050_PWR_CTRL_RESET};
    uint8_t whoami;
    
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    sleep_ms(200);

    buf[1] = MPU6050_PWR_CTRL_WAKE;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    sleep_ms(200);

    reg = MPU6050_WHOAMI_REG;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, &whoami, 1, false);

    if (whoami != 0x68){
        printf("Incorrect device at address. Who Am I reads: 0x%2x\n", whoami);
        
        while (1);
    }
}

// MPU6050 set configs
static void mpu6050_config(uint8_t gyro_config, uint8_t accel_config) {

    //set gyro config
    uint8_t buf[] = {MPU6050_GYRO_CONFIG, gyro_config};
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    //set accelerometer congif
    buf[0] = MPU6050_ACCEL_CONFIG;
    buf[1] = accel_config;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

}

//MPU readdata
static void mpu6050_data_read(int16_t gyro[3], int16_t accel[3], int16_t *temp) {
    
    uint8_t buf[6];
    uint8_t reg;
   //read gyro
    reg = MPU6050_GYRO_READINGS;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buf, 6, false);

    //combine bytes and store in gyro array
    for (int i = 0; i < 3; i++) {
        gyro[i] = (buf[i * 2] << 8 | buf[(i * 2) + 1]);
    }

    //read accel
    reg = MPU6050_ACCEL_READINGS;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buf, 6, false);

    //combine bytes and store in accel array
    for (int i = 0; i < 3; i++) {
        accel[i] = (buf[i * 2] << 8 | buf[(i * 2) + 1]);
    }

    //read temp
    reg = MPU6050_TEMP_READING;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);  

    //combine bytes and store
    *temp = buf[0] << 8 | buf[1];

}

//Calibrate MPU
static void mpu6050_calibrate(int32_t gyro_cal[3], int32_t accel_cal[3]) {
    /*arrays are 32 bit to avoid potential for overflow*/

    int16_t gyro[3] = {0}, accel[3] = {0}, temp ;

    //initial time to settle
    sleep_ms(300);
    printf("Gathering measurements for calibration\n");

    // capture a number of readings and sum them for each axis 
    for (uint8_t i = 0; i < 100; i++) {
        mpu6050_data_read(gyro, accel, &temp);

        gyro_cal[0] += gyro[0];
        gyro_cal[1] += gyro[1];
        gyro_cal[2] += gyro[2];

        accel_cal[0] += accel[0];
        accel_cal[1] += accel[1];
        accel_cal[2] += accel[2];

        //sleep to give some buffer between measurements
        sleep_ms(100);
        
        if (i % 5 == 0 || i == 0){

            printf("values: Ax: %i, Ay: %i, Az: %i, Gx: %i, Gy: %i, Gz: %i\n",
                    accel[0], accel[1], accel[2], gyro[0], gyro[1], gyro[2]);
            
            printf("sum values: Ax: %i, Ay: %i, Az: %i, Gx: %i, Gy: %i, Gz: %i\n",
                    accel_cal[0], accel_cal[1], accel_cal[3], gyro_cal[0], gyro_cal[1], gyro_cal[2]);

        }
    }

    //averaging the measurements
    for (uint8_t i = 0; i < 3; i++) {

        gyro_cal[i] /= 100;
        accel_cal[i] /= 100;

    }
    //account for gravity in calibration
    accel_cal[2] -= (1 * ACCEL_8G_FACTOR);

    printf("Calibration complete\n");
    printf("values: Ax: %i, Ay: %i, Az: %i, Gx: %i, Gy: %i, Gz: %i\n",
            accel_cal[0], accel_cal[1], accel_cal[2], gyro_cal[0], gyro_cal[1], gyro_cal[2]);
    
    printf("values: Ax: %2f, Ay: %2f, Az: %2f, Gx: %2f, Gy: %2f, Gz: %2f\n",
        (float) (accel_cal[0] / ACCEL_8G_FACTOR), 
        (float) (accel_cal[1] / ACCEL_8G_FACTOR),
        (float) (accel_cal[2] / ACCEL_8G_FACTOR), 
        (float) (gyro_cal[0] / GYRO_500DPS_FACTOR), 
        (float) (gyro_cal[1] / GYRO_500DPS_FACTOR),
        (float) (gyro_cal[2] / GYRO_500DPS_FACTOR)
        );
    sleep_ms(5000);        
}