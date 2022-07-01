#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <lgpio.h>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include "io_config.h"

extern "C" {
#include "io.h"
}

class IOController
{
public:
    IOController();
    ~IOController();

    // Initialize IO Controller
    void initialize();

    // Get button states
    bool isButtonShortPressed(uint8_t button);
    bool isButtonLongHeld(uint8_t button);

    // Clear button states
    void clearButtonStates();

    // Get encoder position
    float getEncoderPositionAngle();
    float getEncoderPositionLinear();

    // Get epoch time
    double getTime();

    // Sleep in seconds
    void sleep(double time);

private:
    
    uint8_t gpioChipHandle;

    inputs_t inputs;
};

#endif