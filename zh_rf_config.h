#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        OPEN = 1,
        CLOSE = 0,
        ALARM = 1,
        DRY = 0
    } zh_rf_sensor_status_type_t;

    typedef struct
    {
        int16_t id;
        int16_t type;
        int16_t battery_voltage;
        int16_t value_1;
        int16_t value_2;
        int16_t value_3;
        int16_t value_4;
    } zh_rf_sensor_transmitted_data_t;

#define ZH_RF_SENSOR_TYPE                                                                                                                        \
    DF(ZH_RF_SENSOR_NONE, "")                                                                                                                    \
    DF(ZH_RF_SENSOR_BME280, "bme280")                 /* {id, ZH_RF_SENSOR_BME280, battery_voltage, humidity, temperature, pressure, 0} */       \
    DF(ZH_RF_SENSOR_BMP280, "bmp280")                 /* {id, ZH_RF_SENSOR_BMP280, battery_voltage, temperature, pressure, 0, 0} */              \
    DF(ZH_RF_SENSOR_BME680, "bme680")                 /* {id, ZH_RF_SENSOR_BME680, battery_voltage, humidity, temperature, pressure, quality} */ \
    DF(ZH_RF_SENSOR_TOUCH_SWITCH, "touch_switch")     /* {id, ZH_RF_SENSOR_TOUCH_SWITCH, battery_voltage, 0, 0, 0, 0} */                         \
    DF(ZH_RF_SENSOR_WATER_LEAKAGE, "water_leakage")   /* {id, ZH_RF_SENSOR_WATER_LEAKAGE, battery_voltage, status (ALARM/DRY), 0, 0, 0} */       \
    DF(ZH_RF_SENSOR_PLANT_HUMIDITY, "plant_humidity") /* {id, ZH_RF_SENSOR_PLANT_HUMIDITY, battery_voltage, humidity, 0, 0, 0} */                \
    DF(ZH_RF_SENSOR_OPEN_CLOSE, "open_close")         /* {id, ZH_RF_SENSOR_OPEN_CLOSE, battery_voltage, status (OPEN/CLOSE), 0, 0, 0} */         \
    DF(ZH_RF_SENSOR_MAX, "")

    typedef enum
    {
#define DF(_value, _name) _value,
        ZH_RF_SENSOR_TYPE
#undef DF
    } zh_rf_sensor_type_t;

    const char *get_zh_rf_sensor_type_name(zh_rf_sensor_type_t value)
    {
        switch (value)
        {
#define DF(_value, _name) \
    case _value:          \
        return _name;
            ZH_RF_SENSOR_TYPE
#undef DF
            break;
        default:
            break;
        }
        return "";
    }

#ifdef __cplusplus
}
#endif