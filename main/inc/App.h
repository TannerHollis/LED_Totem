#ifndef APP_H
#define APP_H

#include <Box2D/Box2D.h>
#include <LEDPanel.h>
#include <Accelerometer.h>
#include <stdint.h>
#include <lgpio.h>

#define FRAME_TIME_INC 0.001f
#define FRAME_TIME_THRESHOLD 0.25f

class App
{
public :
	App(uint8_t id, LEDPanel* panel, Accelerometer* accelerometer);
	~App();
	
	// Initialize App
	void initialize();

	// Deinitialize App
	void deInitialize();

	// Update App
	void update();

	// LED Panel pointer
	LEDPanel* panel;
	
	// Accelerometer pointer
	Accelerometer* accelerometer;

private:
	// Process initialization
	virtual void initializeRun() {
		// Do nothing.
	}

	// Process deinitialization
	virtual void deInitializeRun() {
		// Do nothing.
	}

	// Pre-process update
	virtual void preUpdate() {
		// Do nothing.
	}

	// Process update
	virtual void processUpdate() {
		// Do nothing.
	}

	// Post-process update
	virtual void postUpdate() {
		// Do nothing.
	}

	// Process inputs
	virtual void processInputs() {
		// Do nothing.
	}

	// Auto update frame rate
	void autoUpdateFrameRate(double frameTime);

	// Wait and process inputs until next frame
	void waitUntilNextFrame(double startTime);

	// Target frame time
	float targetFrameTime = 1.0f / 60.0f;

	// App ID and initialization flag
	uint8_t appId;
	bool initialized = false;
};

#endif