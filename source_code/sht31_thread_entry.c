#include "sht31_thread.h"
#include <system_thread_entry.h>
/* SHT31 Thread entry function */

//  SHT31 Temperature and Humidity Sensor
#define SHT31_I2C_ADDRESS   0x44  // 011110+SA0+RW - 0x3C or 0x3D

#define Status_CMD_MSB 0xF3
#define Status_CMD_LSB 0x2D

#define HT_CMD_MSB 0x24     // Temperature and humidity read command
#define HT_CMD_LSB 0x00

void Temperature_Humidity_Conversion(float *temperatureC,
                                     float *temperatureF,
                                     float *humidityP
                                     );

float g_temperatureF_sht31;
float g_humidityP_sht31;
float g_temperatureC_sht31;

static uint8_t  measure_data[6];

void sht31_thread_entry(void)
{
    ssp_err_t err;
    uint8_t cmd[2];

    err = g_i2c_temp.p_api->open(g_i2c_temp.p_ctrl, g_i2c_temp.p_cfg);
    APP_ERROR_TRAP(err)

    while (1)
    {
        cmd[0] = HT_CMD_MSB;
        cmd[1] = HT_CMD_LSB;
        err = g_i2c_temp.p_api->write(g_i2c_temp.p_ctrl, cmd, 2, false);
        APP_ERROR_TRAP(err)

        if (err == SSP_SUCCESS) {
            tx_thread_sleep (100);  // 100 msec conversion time

            err = g_i2c_temp.p_api->read(g_i2c_temp.p_ctrl, measure_data, 6, false);
            APP_ERROR_TRAP(err)
            if (err == SSP_SUCCESS) {
                Temperature_Humidity_Conversion(&g_temperatureC_sht31, &g_temperatureF_sht31, &g_humidityP_sht31);
            }
        }
        tx_thread_sleep (100);  // 1000 better. 100 bus error - need to find what err is.
    }
}

void Temperature_Humidity_Conversion(float *temperatureC,
                                     float *temperatureF,
                                     float *humidityP
                                     ) {
    uint16_t temperature_raw;
    uint16_t humidity_raw;

    // Calculate the temperature in C and F
    temperature_raw = measure_data[0];
    temperature_raw = (uint16_t)(temperature_raw << 8) ;
    temperature_raw = (uint16_t)(temperature_raw + (uint16_t) measure_data[1]);
    *temperatureC = (float) (175.0 * (float) temperature_raw / 65535.0);
    *temperatureC = *temperatureC - 45;

    *temperatureF = (float) (315.0 * (float) temperature_raw / 65535.0);
    *temperatureF = *temperatureF - 49;

    if (*temperatureF < 0.1 || *temperatureC < 0.1){
        g_ioport.p_api->pinWrite(g_leds.p_leds[GRN],ON);
    }

    // Calculate the humidity in %
    humidity_raw      = measure_data[3];
    humidity_raw      = (uint16_t) (humidity_raw << 8);
    humidity_raw      = (uint16_t) (humidity_raw + (uint16_t) measure_data[4]);
    *humidityP        = (float) (100.0 * (float) humidity_raw / 65535.0);
}
