#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <Setting.h>
#include <lgpio.h>
#include <cstdint>
#include <cstdlib>
#include <cstdio>

extern "C" {
#include "io.h"
}

class IOController
{
public:
    IOController(Setting* settings);
    ~IOController();

    // Next button handling
    bool isNextLongHeld();
    bool isNextShortPressed();

    // Previous button handling
    bool isPreviousLongHeld();
    bool isPreviousShortPressed();

    // General button handling
    bool isGeneralLongHeld();
    bool isGeneralShortPressed();

    // Rotary encoder button handling
    bool isRotaryButtonLongHeld();
    bool isRotaryButtonShortPressed();

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
    // Initialize IO Controller
    void initialize(Setting* settings);

    // Get button states
    bool isButtonShortPressed(button_t* button);
    bool isButtonLongHeld(button_t* button);

    uint8_t gpioChipHandle;

    inputs_t inputs;
};

#endif