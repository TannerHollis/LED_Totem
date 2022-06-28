#include "Accelerometer.h"
#include <lgpio.h>
#include <stdio.h>
#include <stdlib.h>

Accelerometer::Accelerometer()
{
    MPU6050_initialize(MPU6050_DEFAULT_ADDRESS);
}

Accelerometer::~Accelerometer()
{
    MPU6050_deInitialize();
}

void Accelerometer::updateAccelerometer() {
    MPU6050_getAcceleration(&accelerometer.x.raw, &accelerometer.y.raw, &accelerometer.z.raw);
    accelerometer.x.update_flag = true;
    accelerometer.y.update_flag = true;
    accelerometer.z.update_flag = true;
}

float Accelerometer::aX()
{
    if (!accelerometer.x.update_flag) {
        updateAccelerometer();
    }
    accelerometer.x.update_flag = false;
    switch (accelerometer.scale_setting) {
        case MPU6050_ACCEL_FS_2 :
            return (float)accelerometer.x.raw / 16384.0f;
        case MPU6050_ACCEL_FS_4 :
            return (float)accelerometer.x.raw / 8192.0f;
        case MPU6050_ACCEL_FS_8:
            return (float)accelerometer.x.raw / 4096.0f;
        case MPU6050_ACCEL_FS_16:
            return (float)accelerometer.x.raw / 2048.0f;
        default :
            return 0.0f;
    }
}

float Accelerometer::aY()
{
    if (!accelerometer.y.update_flag) {
        updateAccelerometer();
    }
    accelerometer.y.update_flag = false;
    switch (accelerometer.scale_setting) {
    case MPU6050_ACCEL_FS_2:
        return (float)accelerometer.y.raw / 16384.0f;
    case MPU6050_ACCEL_FS_4:
        return (float)accelerometer.y.raw / 8192.0f;
    case MPU6050_ACCEL_FS_8:
        return (float)accelerometer.y.raw / 4096.0f;
    case MPU6050_ACCEL_FS_16:
        return (float)accelerometer.y.raw / 2048.0f;
    default:
        return 0.0f;
    }
}

float Accelerometer::aZ()
{
    if (!accelerometer.x.update_flag) {
        updateAccelerometer();
    }
    accelerometer.z.update_flag = false;
    switch (accelerometer.scale_setting) {
    case MPU6050_ACCEL_FS_2:
        return (float)accelerometer.z.raw / 16384.0f;
    case MPU6050_ACCEL_FS_4:
        return (float)accelerometer.z.raw / 8192.0f;
    case MPU6050_ACCEL_FS_8:
        return (float)accelerometer.z.raw / 4096.0f;
    case MPU6050_ACCEL_FS_16:
        return (float)accelerometer.z.raw / 2048.0f;
    default:
        return 0.0f;
    }
}

void Accelerometer::updateGyro() {
    MPU6050_getRotation(&gyro.x.raw, &gyro.y.raw, &gyro.z.raw);
    gyro.x.update_flag = true;
    gyro.y.update_flag = true;
    gyro.z.update_flag = true;
}

float Accelerometer::gX()
{
    if (!gyro.x.update_flag) {
        updateGyro();
    }
    gyro.x.update_flag = false;
    switch (gyro.scale_setting) {
    case MPU6050_GYRO_FS_250:
        return (float)gyro.x.raw / 131.0f;
    case MPU6050_GYRO_FS_500:
        return (float)gyro.x.raw / 65.5f;
    case MPU6050_GYRO_FS_1000:
        return (float)gyro.x.raw / 32.8f;
    case MPU6050_GYRO_FS_2000:
        return (float)gyro.x.raw / 16.4f;
    default:
        return 0.0f;
    }
}

float Accelerometer::gY()
{
    if (!gyro.y.update_flag) {
        updateGyro();
    }
    gyro.y.update_flag = false;
    switch (gyro.scale_setting) {
    case MPU6050_GYRO_FS_250:
        return (float)gyro.y.raw / 131.0f;
    case MPU6050_GYRO_FS_500:
        return (float)gyro.y.raw / 65.5f;
    case MPU6050_GYRO_FS_1000:
        return (float)gyro.y.raw / 32.8f;
    case MPU6050_GYRO_FS_2000:
        return (float)gyro.y.raw / 16.4f;
    default:
        return 0.0f;
    }
}

float Accelerometer::gZ()
{
    if (!gyro.z.update_flag) {
        updateGyro();
    }
    gyro.z.update_flag = false;
    switch (gyro.scale_setting) {
    case MPU6050_GYRO_FS_250:
        return (float)gyro.z.raw / 131.0f;
    case MPU6050_GYRO_FS_500:
        return (float)gyro.z.raw / 65.5f;
    case MPU6050_GYRO_FS_1000:
        return (float)gyro.z.raw / 32.8f;
    case MPU6050_GYRO_FS_2000:
        return (float)gyro.z.raw / 16.4f;
    default:
        return 0.0f;
    }
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