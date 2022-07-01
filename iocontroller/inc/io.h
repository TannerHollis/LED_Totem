#ifndef IO_H
#define IO_H

#include "lgpio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"

typedef struct {
    uint8_t pin;
    uint8_t invert;
    uint8_t state;
    double lastUpdateTime;
    uint8_t shortPress;
    uint8_t longHold;
} button_t;

typedef struct {
    button_t next;
    button_t previous;
    button_t general;
    struct {
        struct {
            uint8_t pin;
            uint8_t current;
            uint8_t last;
        } a;
        struct {
            uint8_t pin;
            uint8_t current;
            uint8_t last;
        } b;
        struct {
            float angle;
            float linear;
        } pos;
        button_t button;
        uint8_t updateFlag;
        double lastDetent;
        float ppr;
        float linearInc;
    } rotaryEncoder;

    float inputLongHold;
    float inputShortPress;
    uint32_t debounceTime;
} inputs_t;

uint8_t getButtonShortPress(button_t* button, inputs_t* io);

uint8_t getButtonLongHold(button_t* button, inputs_t* io);

void ioCallback(int e, lgGpioAlert_p evt, void* data);

void updateButton(button_t* button, uint8_t state, inputs_t* io);

void updateRotaryEncoder(inputs_t* io);

#endif