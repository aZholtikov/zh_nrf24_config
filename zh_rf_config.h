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
        int16_t battery;
        int16_t value_1;
        int16_t value_2;
        int16_t value_3;
        int16_t value_4;
        int16_t value_5;
        int16_t value_6;
        int16_t value_7;
        int16_t value_8;
        int16_t value_9;
        int16_t value_10;
    } zh_rf_sensor_transmitted_data_t;

    typedef struct
    {
        uint8_t type;
        uint8_t magic;
        uint8_t token_hi;
        uint8_t token_lo;
        uint8_t id;
    } zh_rf_sensor_pairing_data_t;

#define ZH_RF_PAIRING_PACKET_SIZE 5
#define ZH_RF_PAIRING_REQ_MAGIC 0xA5
#define ZH_RF_PAIRING_ACK_MAGIC 0x5A

    /**
     * @brief Sensor types description.
     *
     * DF(value, name, payload size (in bytes))
     *
     *  - BME280                 12   — {id, type, battery, humidity, temperature, pressure}
     *  - BMP280                 10   — {id, type, battery, temperature, pressure}
     *  - BME680                 14   — {id, type, battery, humidity, temperature, pressure, quality}
     *  - TOUCH_SWITCH           6    — {id, type, battery}
     *  - WATER_LEAKAGE          8    — {id, type, battery, status}
     *  - PLANT_HUMIDITY         8    — {id, type, battery, humidity}
     *  - OPEN_CLOSE             8    — {id, type, battery, status}
     *
     * @note payload size - FULL packet size, including the header (id + type + battery).
     */
#define ZH_RF_SENSOR_TYPE                                \
    DF(ZH_RF_SENSOR_NONE, "", 0)                         \
    DF(ZH_RF_SENSOR_BME280, "bme280", 12)                \
    DF(ZH_RF_SENSOR_BMP280, "bmp280", 10)                \
    DF(ZH_RF_SENSOR_BME680, "bme680", 14)                \
    DF(ZH_RF_SENSOR_TOUCH_SWITCH, "touch_switch", 6)     \
    DF(ZH_RF_SENSOR_WATER_LEAKAGE, "water_leakage", 8)   \
    DF(ZH_RF_SENSOR_PLANT_HUMIDITY, "plant_humidity", 8) \
    DF(ZH_RF_SENSOR_OPEN_CLOSE, "open_close", 8)         \
    DF(ZH_RF_SENSOR_MAX, "", 0)

    typedef enum
    {
#define DF(_value, _name, _size) _value,
        ZH_RF_SENSOR_TYPE
#undef DF
    } zh_rf_sensor_type_t;

    static inline const char *zh_get_rf_sensor_type_name(zh_rf_sensor_type_t value)
    {
        switch (value)
        {
#define DF(_value, _name, _size) \
    case _value:                 \
        return _name;
            ZH_RF_SENSOR_TYPE
#undef DF
            break;
        default:
            break;
        }
        return "";
    }

    static inline uint8_t zh_get_rf_sensor_payload_size(int16_t type)
    {
        switch (type)
        {
#define DF(_value, _name, _size) \
    case _value:                 \
        return (uint8_t)(_size);
            ZH_RF_SENSOR_TYPE
#undef DF
            break;
        default:
            break;
        }
        return 0;
    }

#ifdef __cplusplus
}
#endif