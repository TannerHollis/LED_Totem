# Include headers
include_directories(${WaterSimulation_SOURCE_DIR}/ws2812-rpi/inc)

# Output library
add_library(ws2812-rpi ws2811.c dma.c mailbox.c pcm.c pwm.c rpihw.c)