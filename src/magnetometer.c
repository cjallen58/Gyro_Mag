#include <stdio.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "../libs/magnetometer.h"
#include "../libs/i2c_config.h"

static void mag_config() {

    uint8_t reg = MAG_REG;
    uint8_t buf[2] = {CONFIG_A, CRA_CONFIG};
    uint8_t whoami;

    i2c_write_blocking(I2C_PORT, reg, buf, 2, false);

    sleep_ms(100);
    
    buf[0] = CONFIG_B;
    buf[1] = CRB_CONFIG;

    i2c_write_blocking(I2C_PORT, reg, buf, 2, false);

    sleep_ms(100);

    buf[0] = MODE_REG;
    buf[1] = MODE_CONFIG;

    i2c_write_blocking(I2C_PORT, reg, buf, 2, false);

}

static void mag_read(int16_t mag[3]) {
    
    uint8_t buf[6];
    uint8_t reg = MAG_READINGS;

    //read magnetometer values
    i2c_write_blocking(I2C_PORT, MAG_REG, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MAG_REG, buf, 6, false);

    //condense 6 bytes into 3 signed integers
    for (int i = 0; i < 3; i++) {
        mag[i] = (buf[i * 2] << 8 | buf[(i * 2) + 1]);
    }

}
