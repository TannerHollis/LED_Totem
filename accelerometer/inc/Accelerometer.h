#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "mpu6050.h"

class Accelerometer
{
public:
    Accelerometer();
    ~Accelerometer();

    // Accelerometer data
    float aX();
    float aY();
    float aZ();

    // Gyroscope data
    float gX();
    float gY();
    float gZ();

    // Temperature data
    float tempC();
    float tempF();
    
private:
    void updateAllValues();
    void updateAccelerometer();
    void updateGyro();
    void updateTemp();

    // Accelerometer struct
    struct {
        struct {
            float val;
            int16_t raw;
            bool update_flag = false;
        } x;
        struct {
            float val;
            int16_t raw;
            bool update_flag = false;
        } y;
        struct {
            float val;
            int16_t raw;
            bool update_flag = false;
        } z;
        uint8_t scale_setting;
    } accelerometer;

    // Gyro struct
    struct {
        struct {
            float val;
            int16_t raw;
            bool update_flag = false;
        } x;
        struct {
            float val;
            int16_t raw;
            bool update_flag = false;
        } y;
        struct {
            float val;
            int16_t raw;
            bool update_flag = false;
        } z;
        uint8_t scale_setting;
    } gyro;

    // Temp struct
    struct {
        float val;
        int16_t raw;
        bool update_flag = false;
    } temp;
};

#endif