# Copy config file
file(COPY ${WaterSimulation_SOURCE_DIR}/settings.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../..)

# Add Executable
add_executable(main main.cpp App.cpp)

# Include header files
include_directories(${WaterSimulation_SOURCE_DIR}/Box2D
					${WaterSimulation_SOURCE_DIR}/ws2812-rpi/inc
					${WaterSimulation_SOURCE_DIR}/ledpanel/inc
					${WaterSimulation_SOURCE_DIR}/accelerometer/inc
					${WaterSimulation_SOURCE_DIR}/mpu6050/inc
					${WaterSimulation_SOURCE_DIR}/iocontroller/inc
					${WaterSimulation_SOURCE_DIR}/libconfig/inc
					${WaterSimulation_SOURCE_DIR}/main/inc)

# Link source files
link_directories(${WaterSimulation_SOURCE_DIR}/ws2812-rpi/src
				${WaterSimulation_SOURCE_DIR}/ledpanel/src
				${WaterSimulation_SOURCE_DIR}/accelerometer/src
				${WaterSimulation_SOURCE_DIR}/mpu6050/src
                ${WaterSimulation_SOURCE_DIR}/iocontroller/src
				${WaterSimulation_SOURCE_DIR}/libconfig/src)

# Link libraries
target_link_libraries(main  Box2D
							accelerometer
							ledpanel
							iocontroller
							libconfig
							lgpio)