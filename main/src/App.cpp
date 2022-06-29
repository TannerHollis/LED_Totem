#include "App.h"

App::App(uint8_t id, LEDPanel* panel, Accelerometer* accelerometer)
{
	this->appId = appId;
	this->panel = panel;
	this->accelerometer = accelerometer;
	this->initialized = false;
}

App::~App()
{
	// Do nothing.
}

void App::initialize()
{
	if (!initialized) {
		initialized = true;
		initializeRun();
	}
	else
		return;
}

void App::deInitialize()
{
	if (initialized) {
		initialized = false;
		deInitializeRun();
	}
	else
		return;
}

void App::update()
{
	double startTime = lguTime();
	preUpdate();
	processUpdate();
	postUpdate();
	autoUpdateFrameRate(lguTime() - startTime);
	waitUntilNextFrame(startTime);
}

void App::autoUpdateFrameRate(double frameTime)
{
	if ( frameTime > targetFrameTime ) {
		targetFrameTime += FRAME_TIME_INC;
	}
	if ( frameTime < (targetFrameTime * (1.0f - FRAME_TIME_THRESHOLD) ) ) {
		targetFrameTime -= FRAME_TIME_INC;
	}
}

void App::waitUntilNextFrame(double startTime) {
	while (lguTime() - startTime < targetFrameTime) {
		processInputs();
	}
}