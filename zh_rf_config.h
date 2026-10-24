/**
 * @file zh_rf_config.h
 *
 * @brief RF sensor configuration types and pairing protocol definitions.
 *
 * This header defines data structures and enumerations for RF sensor devices,
 * including transmitted payload, pairing packet format, sensor status values,
 * and a list of supported sensor types with associated metadata. It also
 * provides inline helper functions to query sensor type names and payload
 * sizes from the X-macro table.
 *
 * Key features:
 * - Sensor type enumeration via X-macro (name, payload size)
 * - Transmitted data structure with up to 10 value fields
 * - Pairing request/acknowledge packet format with magic bytes
 * - Binary status types for open/close and alarm/dry sensors
 *
 * @note All transmitted data fields are fixed-width int16_t for wire-format compatibility.
 * @note Payload sizes in the X-macro table include the full packet (header plus sensor values).
 */
#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Binary status values reported by discrete RF sensors.
     *
     * Used by OPEN_CLOSE and WATER_LEAKAGE sensor types. The same underlying
     * values carry different semantics per sensor: OPEN/CLOSE for contact
     * sensors, ALARM/DRY for leak detectors.
     */
    typedef enum
    {
        ZH_RF_SENSOR_STATUS_NONE,  /*!< Status is not set or unknown */
        ZH_RF_SENSOR_STATUS_OPEN,  /*!< Contact sensor is open */
        ZH_RF_SENSOR_STATUS_CLOSE, /*!< Contact sensor is closed */
        ZH_RF_SENSOR_STATUS_ALARM, /*!< Leak detector reports wet/alarm */
        ZH_RF_SENSOR_STATUS_DRY,   /*!< Leak detector reports dry/no-alarm */
        ZH_RF_SENSOR_STATUS_MAX    /*!< Sentinel value marking the end of the enumeration */
    } zh_rf_sensor_status_type_t;

    /**
     * @brief Fixed-width payload transmitted by an RF sensor.
     *
     * Header fields (id, type, battery) are present in every transmission.
     * Value fields are used only by sensor types that report measurements.
     */
    typedef struct
    {
        int16_t id;       /*!< Sensor identifier assigned during pairing */
        int16_t type;     /*!< Sensor type code from zh_rf_sensor_type_t */
        int16_t battery;  /*!< Battery level reported by the sensor */
        int16_t value_1;  /*!< First measurement or status value */
        int16_t value_2;  /*!< Second measurement or status value */
        int16_t value_3;  /*!< Third measurement or status value */
        int16_t value_4;  /*!< Fourth measurement or status value */
        int16_t value_5;  /*!< Fifth measurement or status value */
        int16_t value_6;  /*!< Sixth measurement or status value */
        int16_t value_7;  /*!< Seventh measurement or status value */
        int16_t value_8;  /*!< Eighth measurement or status value */
        int16_t value_9;  /*!< Ninth measurement or status value */
        int16_t value_10; /*!< Tenth measurement or status value */
    } zh_rf_sensor_transmitted_data_t;

    /**
     * @brief States of the RF sensor pairing state machine.
     *
     * Describes the current step of the pairing exchange between a sensor and
     * the hub. The machine cycles through sending a pairing request, waiting
     * for the transmission to complete, listening for the acknowledge, and
     * backing off before retrying when no answer arrived.
     */
    typedef enum
    {
        ZH_RF_PAIR_SEND,    /*!< Build and submit a pairing request packet for transmission */
        ZH_RF_PAIR_TX_WAIT, /*!< Wait until the radio has finished sending the request */
        ZH_RF_PAIR_RX_WAIT, /*!< Listen for the acknowledge packet from the hub */
        ZH_RF_PAIR_BACKOFF  /*!< Idle for a random delay before restarting the pairing attempt */
    } zh_rf_sensor_pairing_state_t;

    /**
     * @brief Pairing packet exchanged between a sensor and the hub.
     *
     * A pairing request carries ZH_RF_PAIRING_REQ_MAGIC as its magic byte;
     * the acknowledge response uses ZH_RF_PAIRING_ACK_MAGIC. Token fields
     * form a 16-bit association identifier split into high and low bytes.
     */
    typedef struct
    {
        uint8_t type;     /*!< Sensor type code being paired */
        uint8_t magic;    /*!< Direction marker (request or acknowledge magic) */
        uint8_t token_hi; /*!< High byte of the pairing token */
        uint8_t token_lo; /*!< Low byte of the pairing token */
        uint8_t id;       /*!< Sensor identifier assigned by the hub */
    } zh_rf_sensor_pairing_data_t;

#define ZH_RF_PAIRING_PACKET_SIZE 5
#define ZH_RF_PAIRING_REQ_MAGIC 0xA5
#define ZH_RF_PAIRING_ACK_MAGIC 0x5A

#define ZH_RF_DATA_HEADER_SIZE 6
#define ZH_RF_DATA_FIELD_SIZE 2
#define ZH_RF_MAX_PAYLOAD_SIZE 32

    /**
     * @brief X-macro table describing every supported RF sensor type.
     *
     * Each entry expands as DF(value, name, payload size (in bytes)):
     *  - BME280                 12   — {id, type, battery, humidity, temperature, pressure}
     *  - BMP280                 10   — {id, type, battery, temperature, pressure}
     *  - BME680                 14   — {id, type, battery, humidity, temperature, pressure, quality}
     *  - TOUCH_SWITCH           6    — {id, type, battery}
     *  - WATER_LEAKAGE          8    — {id, type, battery, status}
     *  - PLANT_HUMIDITY         8    — {id, type, battery, humidity}
     *  - OPEN_CLOSE             8    — {id, type, battery, status}
     *
     * @note Payload size is the full packet size, including the header (id + type + battery).
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

    /**
     * @brief Identifiers of supported RF sensor types.
     *
     * Generated from the ZH_RF_SENSOR_TYPE X-macro table. ZH_RF_SENSOR_NONE
     * marks an unset type and ZH_RF_SENSOR_MAX serves as a sentinel value.
     */
    typedef enum
    {
#define DF(_value, _name, _size) _value,
        ZH_RF_SENSOR_TYPE
#undef DF
    } zh_rf_sensor_type_t;

    /**
     * @brief Retrieve the human-readable name of a sensor type.
     *
     * @param[in] value Sensor type code from zh_rf_sensor_type_t
     *
     * @return Type name string on success
     * @return Empty string if the value has no associated name
     */
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

    /**
     * @brief Retrieve the full payload size of a sensor transmission.
     *
     * @param[in] type Sensor type code from zh_rf_sensor_type_t
     *
     * @return Payload size in bytes (including the id, type, battery header) on success
     * @return Zero if the type has no associated payload size
     */
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

    /**
     * @brief Append a 16-bit integer to the buffer in little-endian byte order.
     *
     * Writes the least significant byte first, then the most significant byte,
     * advancing the position pointer by two bytes.
     *
     * @param[in,out] buf Destination buffer
     * @param[in,out] pos Current write position (must be < buf_max - 1)
     * @param[in] val Value to write
     */
    static inline void zh_rf_sensor_append_i16_le(uint8_t *buf, uint8_t *pos, int16_t val)
    {
        buf[(*pos)++] = (uint8_t)((uint16_t)val & 0xFF);
        buf[(*pos)++] = (uint8_t)(((uint16_t)val >> 8) & 0xFF);
    }

    /**
     * @brief Read a 16-bit integer from the buffer in little-endian byte order.
     *
     * Reads two consecutive bytes starting at the current position, assembling
     * them into a signed 16-bit value with little-endian byte order, then
     * advances the position pointer by two.
     *
     * @param[in] buf Source buffer
     * @param[in,out] pos Current read position (must be < buf_size - 1)
     *
     * @return 16-bit value assembled from the two bytes at the current position
     */
    static inline int16_t zh_rf_sensor_read_i16_le(const uint8_t *buf, uint8_t *pos)
    {
        int16_t value = (int16_t)((uint16_t)buf[*pos] | ((uint16_t)buf[*pos + 1] << 8));
        *pos += 2;
        return value;
    }

    /**
     * @brief Read a 16-bit integer from the buffer at a fixed offset in little-endian byte order.
     *
     * Reads two consecutive bytes starting at the specified offset and assembles
     * them into a signed 16-bit value with little-endian byte order. Does not
     * modify any position pointer.
     *
     * @param[in] buf Source buffer
     * @param[in] offset Byte offset from the start of the buffer (must be < buf_size - 1)
     *
     * @return 16-bit value assembled from the two bytes at the specified offset
     */
    static inline int16_t zh_rf_sensor_read_i16_le_at(const uint8_t *buf, uint8_t offset)
    {
        return (int16_t)((uint16_t)buf[offset] | ((uint16_t)buf[offset + 1] << 8));
    }

    /**
     * @brief Serialize a transmitted data structure into a wire-format payload buffer.
     *
     * Writes the sensor id, type, and battery fields followed by measurement
     * values determined by the sensor type code. Supports TOUCH_SWITCH,
     * WATER_LEAKAGE, OPEN_CLOSE, PLANT_HUMIDITY, BMP280, BME280, and BME680.
     *
     * @param[in] data Pointer to the transmitted data structure (must not be NULL)
     * @param[out] buf Destination buffer for the serialized payload (must not be NULL)
     * @param[in] buf_max Size of the destination buffer in bytes
     *
     * @return Number of bytes written to the buffer on success
     * @return Zero if the buffer is too small for the sensor's payload format
     * @return Zero if the sensor type is unknown
     */
    static inline uint8_t zh_rf_sensor_build_payload(const zh_rf_sensor_transmitted_data_t *data, uint8_t *buf, uint8_t buf_max)
    {
        uint8_t pos = 0;
        if (buf_max < ZH_RF_DATA_HEADER_SIZE)
        {
            return 0;
        }
        zh_rf_sensor_append_i16_le(buf, &pos, data->id);
        zh_rf_sensor_append_i16_le(buf, &pos, data->type);
        zh_rf_sensor_append_i16_le(buf, &pos, data->battery);
        switch (data->type)
        {
        case ZH_RF_SENSOR_TOUCH_SWITCH:
            break;
        case ZH_RF_SENSOR_WATER_LEAKAGE:
        case ZH_RF_SENSOR_OPEN_CLOSE:
        case ZH_RF_SENSOR_PLANT_HUMIDITY:
            if (buf_max < pos + ZH_RF_DATA_FIELD_SIZE)
            {
                return 0;
            }
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_1);
            break;
        case ZH_RF_SENSOR_BMP280:
            if (buf_max < pos + 2 * ZH_RF_DATA_FIELD_SIZE)
            {
                return 0;
            }
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_1);
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_2);
            break;
        case ZH_RF_SENSOR_BME280:
            if (buf_max < pos + 3 * ZH_RF_DATA_FIELD_SIZE)
            {
                return 0;
            }
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_1);
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_2);
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_3);
            break;
        case ZH_RF_SENSOR_BME680:
            if (buf_max < pos + 4 * ZH_RF_DATA_FIELD_SIZE)
            {
                return 0;
            }
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_1);
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_2);
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_3);
            zh_rf_sensor_append_i16_le(buf, &pos, data->value_4);
            break;
        default:
            return 0;
        }
        if (pos != zh_get_rf_sensor_payload_size(data->type))
        {
            return 0;
        }
        return pos;
    }

#ifdef __cplusplus
}
#endif