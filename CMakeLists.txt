# Cmake Minimum Required
cmake_minimum_required(VERSION 3.18)

# Project name
project(WaterSimulation)

# Add MPU6050 directory
add_subdirectory(mpu6050)

# Add WS2812_RPI directory
add_subdirectory(ws2812-rpi)

# Add LEDPanel directory
add_subdirectory(ledpanel)

# Add Accelerometer directory
add_subdirectory(accelerometer)

# Add liquidfun directory
add_subdirectory(Box2D)

# Add main directory
add_subdirectory(main)