# Include headers
include_directories(${WaterSimulation_SOURCE_DIR}/ledpanel/inc
					${WaterSimulation_SOURCE_DIR}/ws2812-rpi/inc
					${WaterSimulation_SOURCE_DIR}/libconfig/inc)

#Include WS2812 source
link_directories(${WaterSimulation_SOURCE_DIR}/ws2812-rpi/src)

# Output library
add_library(ledpanel LEDPanel.cpp)

# Link ledpanel library
target_link_Libraries(ledpanel ws2812-rpi)