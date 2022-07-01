#include "io.h"

uint8_t getButtonShortPress(button_t* button, inputs_t* io)
{
    return (lguTime() - button->lastUpdateTime > io->inputShortPress) || !getButtonLongHold(button, io);
}

uint8_t getButtonLongHold(button_t* button, inputs_t* io)
{
    return lguTime() - button->lastUpdateTime > io->inputLongHold;
}

void ioCallback(int e, lgGpioAlert_p evt, void* data)
{
    inputs_t* io = (inputs_t*)data;
    for (uint8_t i = 0; i < e; i++) {
        char gpio = evt[i].report.gpio;
        if (gpio == io->next.pin) {
            updateButton(&io->next, evt[i].report.level, io);
        }
        if (gpio == io->previous.pin) {
            updateButton(&io->previous, evt[i].report.level, io);
        }
        if (gpio == io->general.pin) {
            updateButton(&io->general, evt[i].report.level, io);
        }
        if (gpio == io->rotaryEncoder.button.pin) {
            updateButton(&io->rotaryEncoder.button, evt[i].report.level, io);
        }
        if (gpio == io->rotaryEncoder.a.pin) {
            io->rotaryEncoder.a.current = evt[i].report.level;
            io->rotaryEncoder.updateFlag |= 1;
            io->rotaryEncoder.lastDetent = lguTime();
        }
        if (gpio == io ->rotaryEncoder.b.pin) {
            io->rotaryEncoder.b.current = evt[i].report.level;
            io->rotaryEncoder.updateFlag |= 1;
            io->rotaryEncoder.lastDetent = lguTime();
        }
    }
    if (io->rotaryEncoder.updateFlag) {
        updateRotaryEncoder(io);
    }
}

void updateButton(button_t* button, uint8_t state, inputs_t* io)
{
    button->state = state;
    if ((button->state && !button->invert) || (!button->state && button->invert)) {
        button->shortPress = 0;
        button->longHold = 0;
    }
    else {
        button->shortPress = getButtonShortPress(button, io);
        button->longHold = getButtonLongHold(button, io);
    }
    button->lastUpdateTime = lguTime();
    return;
}

void updateRotaryEncoder(inputs_t* io)
{
    // Save encoder states
    uint8_t currentState = (io->rotaryEncoder.b.current << 1) | io->rotaryEncoder.a.current;
    uint8_t lastState = (io->rotaryEncoder.b.last << 1) | io->rotaryEncoder.a.last;

    // Declare direction
    float direction = 0;

    // Not my problem
    switch (lastState)
    {
    case 0:
        switch (currentState)
        {
        case 0:
            direction = 0;
            break;
        case 1:
            direction = 1;
            break;
        case 2:
            direction = -1;
            break;
        case 3:
            direction = 0;
            break;
        }
        break;
    case 1:
        switch (currentState)
        {
        case 0:
            direction = -1;
            break;
        case 1:
            direction = 0;
            break;
        case 2:
            direction = 0;
            break;
        case 3:
            direction = 1;
            break;
        }
        break;
    case 2:
        switch (currentState)
        {
        case 0:
            direction = 1;
            break;
        case 1:
            direction = 0;
            break;
        case 2:
            direction = 0;
            break;
        case 3:
            direction = -1;
            break;
        }
        break;
    case 3:
        switch (currentState)
        {
        case 0:
            direction = 0;
            break;
        case 1:
            direction = -1;
            break;
        case 2:
            direction = 1;
            break;
        case 3:
            direction = 0;
            break;
        }
        break;
    }

    // Add to rotary encoder linear position
    io->rotaryEncoder.pos.linear += direction * io->rotaryEncoder.linearInc;
    if (io->rotaryEncoder.pos.linear > 1.0f)
        io->rotaryEncoder.pos.linear = 1.0f;
    else if (io->rotaryEncoder.pos.linear < 0.0f)
        io->rotaryEncoder.pos.linear = 0.0f;

    // Add to rotary encoder angular position
    io->rotaryEncoder.pos.angle += direction / (720.0f / io->rotaryEncoder.ppr);
    if (io->rotaryEncoder.pos.linear > 360.0f)
        io->rotaryEncoder.pos.linear = 0.0f;
    else if (io->rotaryEncoder.pos.linear < 0.0f)
        io->rotaryEncoder.pos.linear = 360.0f;

    // Shift current values to last 
    io->rotaryEncoder.a.last = io->rotaryEncoder.a.current;
    io->rotaryEncoder.b.last = io->rotaryEncoder.b.current;

    // Clear update flag
    io->rotaryEncoder.updateFlag = 0;
}