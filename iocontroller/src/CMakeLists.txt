# Include headers
include_directories(${WaterSimulation_SOURCE_DIR}/iocontroller/inc)

# Output library
add_library(iocontroller IOController.cpp io.c)

# Link lgpio library
target_link_Libraries(iocontroller lgpio)