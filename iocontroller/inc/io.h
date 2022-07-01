#ifndef IO_H
#define IO_H

#include "io_config.h"
#include "lgpio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"

typedef struct {
    struct {
        uint8_t state;
        double lastUpdateTime;
        uint8_t shortPress;
        uint8_t longHold;
    } next;
    struct {
        uint8_t state;
        double lastUpdateTime;
        uint8_t shortPress;
        uint8_t longHold;
    } previous;
    struct {
        uint8_t state;
        double lastUpdateTime;
        uint8_t shortPress;
        uint8_t longHold;
    } general;
    struct {
        struct {
            uint8_t current;
            uint8_t last;
        } a;
        struct {
            uint8_t current;
            uint8_t last;
        } b;
        struct {
            float angle;
            float linear;
        } pos;
        struct {
            uint8_t state;
            double lastUpdateTime;
            uint8_t shortPress;
            uint8_t longHold;
        } button;
        uint8_t updateFlag;
        double lastDetent;
    } rotaryEncoder;
} inputs_t;

uint8_t getButtonShortPress(double lastUpdateTime);

uint8_t getButtonLongHold(double lastUpdateTime);

void ioCallback(int e, lgGpioAlert_p evt, void* data);

void updateRotaryEncoder(inputs_t* inputs);

#endif