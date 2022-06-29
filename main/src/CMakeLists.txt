# Add Executable
add_executable(main main.cpp)

# Include header files
include_directories(${WaterSimulation_SOURCE_DIR}/Box2D
					${WaterSimulation_SOURCE_DIR}/ws2812-rpi/inc
					${WaterSimulation_SOURCE_DIR}/ledpanel/inc
					${WaterSimulation_SOURCE_DIR}/accelerometer/inc
					${WaterSimulation_SOURCE_DIR}/mpu6050/inc
					${WaterSimulation_SOURCE_DIR}/main/inc)

# Link source files
link_directories(${WaterSimulation_SOURCE_DIR}/ws2812-rpi/src
				${WaterSimulation_SOURCE_DIR}/ledpanel/src
				${WaterSimulation_SOURCE_DIR}/accelerometer/src
				${WaterSimulation_SOURCE_DIR}/mpu6050/src)

# Link libraries
target_link_libraries(main  Box2D
							lgpio)

# Link accelerometer library
target_link_libraries(main accelerometer mpu6050 lgpio)

# Link ledpanel library
target_link_libraries(main ledpanel ws2812-rpi)