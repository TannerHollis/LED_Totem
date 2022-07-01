#include "io.h"

uint8_t getButtonShortPress(double lastUpdateTime)
{
    return (lguTime() - lastUpdateTime > INPUT_SHORT_PRESS) || !getButtonLongHold(lastUpdateTime);
}

uint8_t getButtonLongHold(double lastUpdateTime)
{
    return lguTime() - lastUpdateTime > INPUT_LONG_HOLD;
}

void ioCallback(int e, lgGpioAlert_p evt, void* data)
{
    inputs_t* io = (inputs_t*)data;
    for (uint8_t i = 0; i < e; i++) {
        switch (evt[i].report.gpio)
        {
        case INPUT_BTTN_NEXT:
            io->next.state = evt[i].report.level;
            if ((io->next.state && INPUT_BTTN_NEXT_STATE) || (!io->next.state && !INPUT_BTTN_NEXT_STATE)) {
                io->next.shortPress = 0;
                io->next.longHold = 0;
            }
            else {
                io->next.shortPress = getButtonShortPress(io->next.lastUpdateTime);
                io->next.longHold = getButtonLongHold(io->next.lastUpdateTime);
            }
            io->next.lastUpdateTime = lguTime();
            return;
        case INPUT_BTTN_PREVIOUS:
            io->previous.state = evt[i].report.level;
            if ((io->previous.state && INPUT_BTTN_PREVIOUS_STATE) || (!io->previous.state && !INPUT_BTTN_PREVIOUS_STATE)) {
                io->previous.shortPress = 0;
                io->previous.longHold = 0;
            }
            else {
                io->previous.shortPress = getButtonShortPress(io->previous.lastUpdateTime);
                io->previous.longHold = getButtonLongHold(io->previous.lastUpdateTime);
            }
            io->previous.lastUpdateTime = lguTime();
            return;
        case INPUT_BTTN_GENERAL:
            io->general.state = evt[i].report.level;
            if ((io->general.state && INPUT_BTTN_GENERAL_STATE) || (!io->general.state && !INPUT_BTTN_GENERAL_STATE)) {
                io->general.shortPress = 0;
                io->general.longHold = 0;
            }
            else {
                io->general.shortPress = getButtonShortPress(io->general.lastUpdateTime);
                io->general.longHold = getButtonLongHold(io->general.lastUpdateTime);
            }
            io->general.lastUpdateTime = lguTime();
            return;
        case INPUT_BTTN_ENCODER :
            io->rotaryEncoder.button.state = evt[i].report.level;
            if ((io->rotaryEncoder.button.state && INPUT_BTTN_ENCODER_STATE) || (!io->rotaryEncoder.button.state && !INPUT_BTTN_ENCODER_STATE)) {
                io->rotaryEncoder.button.shortPress = 0;
                io->rotaryEncoder.button.longHold = 0;
            }
            else {
                io->rotaryEncoder.button.shortPress = getButtonShortPress(io->rotaryEncoder.button.lastUpdateTime);
                io->rotaryEncoder.button.longHold = getButtonLongHold(io->rotaryEncoder.button.lastUpdateTime);
            }
            io->rotaryEncoder.button.lastUpdateTime = lguTime();
            return;
        case INPUT_ENCODER_A:
            io->rotaryEncoder.a.current = evt[i].report.level;
            io->rotaryEncoder.updateFlag |= 1;
            io->rotaryEncoder.lastDetent = lguTime();
            return;
        case INPUT_ENCODER_B:
            io->rotaryEncoder.b.current = evt[i].report.level;
            io->rotaryEncoder.updateFlag |= 1;
            io->rotaryEncoder.lastDetent = lguTime();
            return;
        default:
            return;
        }
    }
    if (io->rotaryEncoder.updateFlag) {
        updateRotaryEncoder(io);
    }
}

void updateRotaryEncoder(inputs_t* inputs) {
    // Save encoder states
    uint8_t currentState = (inputs->rotaryEncoder.b.current << 1) | inputs->rotaryEncoder.a.current;
    uint8_t lastState = (inputs->rotaryEncoder.b.last << 1) | inputs->rotaryEncoder.a.last;

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
    inputs->rotaryEncoder.pos.linear += direction * ENCODER_LINEAR_INC;
    if (inputs->rotaryEncoder.pos.linear > 1.0f)
        inputs->rotaryEncoder.pos.linear = 1.0f;
    else if (inputs->rotaryEncoder.pos.linear < 0.0f)
        inputs->rotaryEncoder.pos.linear = 0.0f;

    // Add to rotary encoder angular position
    inputs->rotaryEncoder.pos.angle += direction * 1.0f / (2 * ENCODER_PPR);
    if (inputs->rotaryEncoder.pos.linear > 360.0f)
        inputs->rotaryEncoder.pos.linear = 0.0f;
    else if (inputs->rotaryEncoder.pos.linear < 0.0f)
        inputs->rotaryEncoder.pos.linear = 360.0f;

    // Shift current values to last 
    inputs->rotaryEncoder.a.last = inputs->rotaryEncoder.a.current;
    inputs->rotaryEncoder.b.last = inputs->rotaryEncoder.b.current;

    // Clear update flag
    inputs->rotaryEncoder.updateFlag = 0;
}