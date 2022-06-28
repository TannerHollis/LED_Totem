# Include headers
include_directories(${WaterSimulation_SOURCE_DIR}/mpu6050/inc)

# Output library
add_library(mpu6050 mpu6050.c)

# Link lgpio library
target_link_libraries(mpu6050 lgpio)