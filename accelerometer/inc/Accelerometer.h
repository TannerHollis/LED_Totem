#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Setting.h>
#include <lgpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

extern "C" {
#include "mpu6050.h"
}

class Accelerometer
{
public:
    Accelerometer(Setting* settings);
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

    // Auto-scaling technique
    void accelerometerAutoScale();
    void setAccelerometerAutoScale(float threshold);
    void gyroAutoScale();
    void setGyroAutoScale(float threshold);
    
private:
    void updateAccelerometer();
    void updateGyro();
    void updateTemp();

    // Scale raw data
    float scaleAccelerometerRaw(int16_t raw);
    float scaleGyroRaw(int16_t raw);

    // Accelerometer scaling
    void getAccelerometerScaling();
    void setAccelerometerScaling(uint8_t scale);

    // Gyroscope scaling
    void getGyroScaling();
    void setGyroScaling(uint8_t scale);

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
        int16_t auto_scale = INT16_MAX;
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
        int16_t auto_scale = INT16_MAX;
    } gyro;

    // Temp struct
    struct {
        float val;
        int16_t raw;
        bool update_flag = false;
    } temp;
};

#endif