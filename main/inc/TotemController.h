#ifndef TOTEMCONTROLLER_H
#define TOTEMCONTROLLER_H

#include <LEDPanel.h>
#include <Accelerometer.h>
#include <IOController.h>
#include <Setting.h>

typedef struct {
	Setting* settings;
	LEDPanel* panel;
	IOController* ioc;
	Accelerometer* mpu;
	bool running;
} TotemController_t;

#endif