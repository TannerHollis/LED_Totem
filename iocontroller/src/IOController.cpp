#include <IOController.h>

IOController::IOController(Setting* settings)
{  
    gpioChipHandle = lgGpiochipOpen(0);
    if (gpioChipHandle < 0) {
        printf("I/O Controller initialization failed.\n");
    }
    else {
        printf("I/O Controller initialization success!\n");
        initialize(settings);
    }
}

IOController::~IOController()
{
    lgGpiochipClose(gpioChipHandle);
}

void IOController::initialize(Setting* settings)
{
    // Initialize inputs struct
    inputs.next.pin = settings->getUInt("iocontroller.nextButton.pin");
    inputs.next.invert = settings->getBool("iocontroller.nextButton.invert");
    inputs.previous.pin = settings->getUInt("iocontroller.previousButton.pin");
    inputs.previous.invert = settings->getBool("iocontroller.previousButton.invert");
    inputs.general.pin = settings->getUInt("iocontroller.generalButton.pin");
    inputs.general.invert = settings->getBool("iocontroller.generalButton.invert");
    inputs.rotaryEncoder.button.pin = settings->getUInt("iocontroller.rotaryEncoder.button.pin");
    inputs.rotaryEncoder.button.invert = settings->getBool("iocontroller.rotaryEncoder.button.invert");
    inputs.rotaryEncoder.a.pin = settings->getUInt("iocontroller.rotaryEncoder.a.pin");
    inputs.rotaryEncoder.b.pin = settings->getUInt("iocontroller.rotaryEncoder.b.pin");
    inputs.rotaryEncoder.ppr = settings->getFloat("iocontroller.rotaryEncoder.ppr");
    inputs.rotaryEncoder.linearInc = settings->getFloat("iocontroller.rotaryEncoder.linearInc");
    inputs.inputLongHold = settings->getFloat("iocontroller.longHoldTime");
    inputs.inputShortPress = settings->getFloat("iocontroller.shortPressTime");
    inputs.debounceTime = settings->getInt32("iocontroller.debounceTimeMicroSeconds");

    // Add callback function
    lgGpioSetSamplesFunc(ioCallback, &inputs);

    // Claim interrupts for all gpio used
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, inputs.next.pin, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, inputs.previous.pin, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, inputs.general.pin, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, inputs.rotaryEncoder.button.pin, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, inputs.rotaryEncoder.a.pin, -1);
    lgGpioClaimAlert(gpioChipHandle, 0, LG_BOTH_EDGES, inputs.rotaryEncoder.b.pin, -1);

    // Set debounce times for buttons
    lgGpioSetDebounce(gpioChipHandle, inputs.next.pin, inputs.debounceTime);
    lgGpioSetDebounce(gpioChipHandle, inputs.previous.pin, inputs.debounceTime);
    lgGpioSetDebounce(gpioChipHandle, inputs.general.pin, inputs.debounceTime);
    lgGpioSetDebounce(gpioChipHandle, inputs.rotaryEncoder.button.pin, inputs.debounceTime);

    // Clear states and positions
    clearButtonStates();
}

bool IOController::isNextShortPressed()
{
    return isButtonShortPressed(&inputs.next);
}

bool IOController::isNextLongHeld()
{
    return isButtonLongHeld(&inputs.next);
}

bool IOController::isPreviousShortPressed()
{
    return isButtonShortPressed(&inputs.previous);
}

bool IOController::isPreviousLongHeld()
{
    return isButtonLongHeld(&inputs.previous);
}

bool IOController::isGeneralShortPressed()
{
    return isButtonShortPressed(&inputs.general);
}

bool IOController::isGeneralLongHeld()
{
    return isButtonLongHeld(&inputs.general);
}

bool IOController::isRotaryButtonShortPressed()
{
    return isButtonShortPressed(&inputs.rotaryEncoder.button);
}

bool IOController::isRotaryButtonLongHeld()
{
    return isButtonLongHeld(&inputs.rotaryEncoder.button);
}

bool IOController::isButtonShortPressed(button_t* button) 
{
    if (button->shortPress) {
        button->shortPress = false;
        return true;
    }
    else {
        return false;
    }
}

bool IOController::isButtonLongHeld(button_t* button)
{
    if (button->longHold) {
        button->longHold = false;
        return true;
    }
    else {
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
    inputs.rotaryEncoder.pos.angle = 0.0f;
    inputs.rotaryEncoder.pos.linear = 0.5f;
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