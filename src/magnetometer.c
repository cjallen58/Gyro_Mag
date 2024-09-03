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

