# Include headers
include_directories(${WaterSimulation_SOURCE_DIR}/accelerometer/inc
					${WaterSimulation_SOURCE_DIR}/mpu6050/inc)

#Include WS2812 source
link_directories(${WaterSimulation_SOURCE_DIR}/mpu6050/src)

# Output library
add_library(accelerometer Accelerometer.cpp)

# Link accelerometer library
target_link_Libraries(accelerometer mpu6050)