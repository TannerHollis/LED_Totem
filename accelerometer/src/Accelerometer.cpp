#include "Accelerometer.h"
#include <lgpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

Accelerometer::Accelerometer()
{
    MPU6050_initialize(MPU6050_DEFAULT_ADDRESS);
    getAccelerometerScaling();
    getGyroScaling();
}

Accelerometer::~Accelerometer()
{
    MPU6050_deInitialize();
}

void Accelerometer::updateAccelerometer() {
    accelerometerAutoScale();
    MPU6050_getAcceleration(&accelerometer.x.raw, &accelerometer.y.raw, &accelerometer.z.raw);
    accelerometer.x.update_flag = true;
    accelerometer.y.update_flag = true;
    accelerometer.z.update_flag = true;
}

float Accelerometer::scaleAccelerometerRaw(int16_t raw)
{
    switch (accelerometer.scale_setting) {
    case MPU6050_ACCEL_FS_2:
        return (float)raw / 16384.0f;
    case MPU6050_ACCEL_FS_4:
        return (float)raw / 8192.0f;
    case MPU6050_ACCEL_FS_8:
        return (float)raw / 4096.0f;
    case MPU6050_ACCEL_FS_16:
        return (float)raw / 2048.0f;
    default:
        return 0.0f;
    }
}

float Accelerometer::aX()
{
    if (!accelerometer.x.update_flag) {
        updateAccelerometer();
    }
    accelerometer.x.update_flag = false;
    return scaleAccelerometerRaw(accelerometer.x.raw);
}

float Accelerometer::aY()
{
    if (!accelerometer.y.update_flag) {
        updateAccelerometer();
    }
    accelerometer.y.update_flag = false;
    return scaleAccelerometerRaw(accelerometer.y.raw);
}

float Accelerometer::aZ()
{
    if (!accelerometer.x.update_flag) {
        updateAccelerometer();
    }
    accelerometer.z.update_flag = false;
    return scaleAccelerometerRaw(accelerometer.z.raw);
}

void Accelerometer::updateGyro() {
    gyroAutoScale();
    MPU6050_getRotation(&gyro.x.raw, &gyro.y.raw, &gyro.z.raw);
    gyro.x.update_flag = true;
    gyro.y.update_flag = true;
    gyro.z.update_flag = true;
}

float Accelerometer::scaleGyroRaw(int16_t raw)
{
    switch (gyro.scale_setting) {
    case MPU6050_GYRO_FS_250:
        return (float)raw / 131.0f;
    case MPU6050_GYRO_FS_500:
        return (float)raw / 65.5f;
    case MPU6050_GYRO_FS_1000:
        return (float)raw / 32.8f;
    case MPU6050_GYRO_FS_2000:
        return (float)raw / 16.4f;
    default:
        return 0.0f;
    }
}

float Accelerometer::gX()
{
    if (!gyro.x.update_flag) {
        updateGyro();
    }
    gyro.x.update_flag = false;
    return scaleGyroRaw(gyro.x.raw);
}

float Accelerometer::gY()
{
    if (!gyro.y.update_flag) {
        updateGyro();
    }
    gyro.y.update_flag = false;
    return scaleGyroRaw(gyro.y.raw);
}

float Accelerometer::gZ()
{
    if (!gyro.z.update_flag) {
        updateGyro();
    }
    gyro.z.update_flag = false;
    return scaleGyroRaw(gyro.z.raw);
}

void Accelerometer::updateTemp()
{
    temp.raw = MPU6050_getTemperature();
    temp.update_flag = true;
}

float Accelerometer::tempC()
{
    if (!temp.update_flag) {
        updateTemp();
    }
    temp.update_flag = false;
    return (float)temp.raw / 340.0f + 36.53f;
}

float Accelerometer::tempF()
{
    return tempC() * 1.8f + 32.0f;
}

void Accelerometer::getAccelerometerScaling() 
{
    accelerometer.scale_setting = MPU6050_getFullScaleAccelRange();
}

void Accelerometer::setAccelerometerScaling(uint8_t scale) 
{
    MPU6050_setFullScaleAccelRange(scale);
    getAccelerometerScaling();
    printf("Accel scaling changed to %i\n", scale);
}

void Accelerometer::getGyroScaling() 
{
    gyro.scale_setting = MPU6050_getFullScaleGyroRange();
}

void Accelerometer::setGyroScaling(uint8_t scale) 
{
    MPU6050_setFullScaleGyroRange(scale);
    getGyroScaling();
}

void Accelerometer::accelerometerAutoScale()
{
    int16_t threshold = accelerometer.auto_scale;
    int16_t x = abs(accelerometer.x.raw);
    int16_t y = abs(accelerometer.y.raw);
    int16_t z = abs(accelerometer.z.raw);
    bool up_scale = (x > threshold) || (y > threshold) || (z > threshold);
    bool down_scale = (x < INT16_MAX - threshold) && (y < INT16_MAX - threshold) && (z < INT16_MAX - threshold);
    if (up_scale) {
        switch (accelerometer.scale_setting)
        {
            case MPU6050_ACCEL_FS_2 :
                setAccelerometerScaling(MPU6050_ACCEL_FS_4);
                break;
            case MPU6050_ACCEL_FS_4 :
                setAccelerometerScaling(MPU6050_ACCEL_FS_8);
                break;
            case MPU6050_ACCEL_FS_8:
                setAccelerometerScaling(MPU6050_ACCEL_FS_16);
                break;
            case MPU6050_ACCEL_FS_16:
                break;
        }
    }
    else if (down_scale) {
        switch (accelerometer.scale_setting)
        {
        case MPU6050_ACCEL_FS_2:
            break;
        case MPU6050_ACCEL_FS_4:
            setAccelerometerScaling(MPU6050_ACCEL_FS_2);
            break;
        case MPU6050_ACCEL_FS_8:
            setAccelerometerScaling(MPU6050_ACCEL_FS_4);
            break;
        case MPU6050_ACCEL_FS_16:
            setAccelerometerScaling(MPU6050_ACCEL_FS_8);
            break;
        }
    }
}

void Accelerometer::setAccelerometerAutoScale(float threshold)
{
    if (threshold > 1.0f) {
        accelerometer.auto_scale = INT16_MAX;
    }
    else if (threshold < 0.0f) {
        accelerometer.auto_scale = INT16_MAX / 2;
    }
    else {
        accelerometer.auto_scale = (1.0f + threshold) / 2.0f * INT16_MAX;
    }
    printf("Accel auto scale set to %i\n", accelerometer.auto_scale);
}

void Accelerometer::gyroAutoScale()
{
    int16_t threshold = gyro.auto_scale;
    int16_t x = abs(gyro.x.raw);
    int16_t y = abs(gyro.y.raw);
    int16_t z = abs(gyro.z.raw);
    bool up_scale = x > threshold || y > threshold || z > threshold;
    bool down_scale = x < INT16_MAX - threshold || y < INT16_MAX - threshold || z < INT16_MAX - threshold;
    if (up_scale) {
        switch (gyro.scale_setting)
        {
            case MPU6050_GYRO_FS_250:
                setGyroScaling(MPU6050_GYRO_FS_500);
                break;
            case MPU6050_GYRO_FS_500:
                setGyroScaling(MPU6050_GYRO_FS_1000);
                break;
            case MPU6050_GYRO_FS_1000:
                setGyroScaling(MPU6050_GYRO_FS_2000);
                break;
            case MPU6050_GYRO_FS_2000:
                break;
        }
    }
    else if (down_scale) {
        switch (gyro.scale_setting)
        {
        case MPU6050_GYRO_FS_250:
            break;
        case MPU6050_GYRO_FS_500:
            setGyroScaling(MPU6050_GYRO_FS_250);
            break;
        case MPU6050_GYRO_FS_1000:
            setGyroScaling(MPU6050_GYRO_FS_500);
            break;
        case MPU6050_GYRO_FS_2000:
            setGyroScaling(MPU6050_GYRO_FS_1000);
            break;
        }
    }
}

void Accelerometer::setGyroAutoScale(float threshold)
{
    if (threshold > 1.0f) {
        gyro.auto_scale = INT16_MAX;
    }
    else if (threshold < 0.0f) {
        gyro.auto_scale = INT16_MAX / 2;
    }
    else {
        gyro.auto_scale = (1.0f + threshold) / 2.0f * INT16_MAX;
    }
}