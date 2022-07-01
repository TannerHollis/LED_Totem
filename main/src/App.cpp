#include "App.h"

App::App(uint8_t id, LEDPanel* panel, Accelerometer* accelerometer, IOController* ioc)
{
	this->appId = appId;
	this->panel = panel;
	this->accelerometer = accelerometer;
	this->ioc = ioc;
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

int8_t App::loop() {
    // Enable running flag
    running = true;

    // Clear I/O states
    ioc->clearButtonStates();
    
    initialize();
    while (running) {
        update();
    }
    deInitialize();

	// Print ending frame rate
	printf("\tFramerate: %4.2f\n", 1.0f / getFrameTime());

	// Clear panel and refresh
	panel->clearPanel();
	panel->display();

    // Add brief delay before exiting
    ioc->sleep(APP_SWITCH_TIME);

    // Return exit code
    return exitCode;
}

void App::stop(int8_t exitCode) {
    running = false;
    this->exitCode = exitCode;
}

void App::update()
{
	double startTime = ioc->getTime();
	preUpdate();
	processUpdate();
	postUpdate();
	autoUpdateFrameRate(ioc->getTime() - startTime);
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

float App::getFrameTime()
{
	return targetFrameTime;
}

void App::waitUntilNextFrame(double startTime) {
	while (ioc->getTime() - startTime < targetFrameTime) {
		processInputs();
	}
}