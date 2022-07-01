#include <IOController.h>

IOController::IOController()
{  
    gpioChipHandle = lgGpiochipOpen(0);
    if (gpioChipHandle < 0) {
        printf("I/O Controller initialization failed.\n");
    }
    else {
        printf("I/O Controller initialization success!\n");
        initialize();
    }
}

IOController::~IOController()
{
    lgGpiochipClose(gpioChipHandle);
}

void IOController::initialize()
{
    // Add callback function
    lgGpioSetSamplesFunc(ioCallback, &inputs);

    // Claim interrupts for all gpio used
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, INPUT_BTTN_NEXT, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, INPUT_BTTN_PREVIOUS, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, INPUT_BTTN_GENERAL, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, INPUT_BTTN_ENCODER, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, INPUT_ENCODER_A, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, INPUT_ENCODER_B, -1);

    // Set debounce times for buttons
    lgGpioSetDebounce(gpioChipHandle, INPUT_BTTN_NEXT, INPUT_DEBOUNCE_TIME);
    lgGpioSetDebounce(gpioChipHandle, INPUT_BTTN_PREVIOUS, INPUT_DEBOUNCE_TIME);
    lgGpioSetDebounce(gpioChipHandle, INPUT_BTTN_GENERAL, INPUT_DEBOUNCE_TIME);
    lgGpioSetDebounce(gpioChipHandle, INPUT_BTTN_ENCODER, INPUT_DEBOUNCE_TIME);
}

bool IOController::isButtonShortPressed(uint8_t button) 
{
    switch (button)
    {
        case INPUT_BTTN_NEXT :
            if (inputs.next.shortPress) {
                inputs.next.shortPress = false;
                return true;
            }
            else {
                return false;
            }
            break;
        case INPUT_BTTN_PREVIOUS :
            if (inputs.previous.shortPress) {
                inputs.previous.shortPress = false;
                return true;
            }
            else {
                return false;
            }
            break;
        case INPUT_BTTN_GENERAL :
            if (inputs.general.shortPress) {
                inputs.general.shortPress = false;
                return true;
            }
            else {
                return false;
            }
            break;
        case INPUT_BTTN_ENCODER :
            if (inputs.rotaryEncoder.button.shortPress) {
                inputs.rotaryEncoder.button.shortPress = false;
                return true;
            }
            else {
                return false;
            }
            break;
        default :
            return false;
    }
}

bool IOController::isButtonLongHeld(uint8_t button)
{
    switch (button)
    {
    case INPUT_BTTN_NEXT:
        if (inputs.next.longHold) {
            inputs.next.longHold = false;
            return true;
        }
        else {
            return false;
        }
        break;
    case INPUT_BTTN_PREVIOUS:
        if (inputs.previous.longHold) {
            inputs.previous.longHold = false;
            return true;
        }
        else {
            return false;
        }
        break;
    case INPUT_BTTN_GENERAL:
        if (inputs.general.longHold) {
            inputs.general.longHold = false;
            return true;
        }
        else {
            return false;
        }
        break;
    case INPUT_BTTN_ENCODER:
        if (inputs.rotaryEncoder.button.longHold) {
            inputs.rotaryEncoder.button.longHold = false;
            return true;
        }
        else {
            return false;
        }
        break;
    default:
        return false;
    }
}

void IOController::clearButtonStates()
{
    inputs.next.shortPress = 0;
    inputs.next.longHold = 0;
    inputs.next.lastUpdateTime = getTime();
    inputs.previous.shortPress = 0;
    inputs.previous.longHold = 0;
    inputs.previous.lastUpdateTime = getTime();
    inputs.general.shortPress = 0;
    inputs.general.longHold = 0;
    inputs.general.lastUpdateTime = getTime();
    inputs.rotaryEncoder.button.shortPress = 0;
    inputs.rotaryEncoder.button.longHold = 0;
    inputs.rotaryEncoder.button.lastUpdateTime = getTime();
}

float IOController::getEncoderPositionAngle()
{
    return inputs.rotaryEncoder.pos.angle;
}

float IOController::getEncoderPositionLinear()
{
    return inputs.rotaryEncoder.pos.linear;
}

double IOController::getTime()
{
    return lguTime();
}

void IOController::sleep(double time)
{
    lguSleep(time);
}