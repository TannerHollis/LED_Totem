#ifndef APP_H
#define APP_H

#include <Box2D/Box2D.h>
#include <LEDPanel.h>
#include <Accelerometer.h>
#include <IOController.h>
#include <lgpio.h>
#include <stdint.h>

#define FRAME_TIME_INC 0.001f
#define FRAME_TIME_THRESHOLD 0.25f
#define APP_SWITCH_TIME 0.05f

class App
{
public :
	App(uint8_t id, LEDPanel* panel, Accelerometer* accelerometer, IOController* ioc);
	~App();
	
    // Loop App
    int8_t loop();

    // Stop App
    void stop(int8_t exitCode);

	// Get frame time
	float getFrameTime();

	// LED Panel pointer
	LEDPanel* panel;
	
	// Accelerometer pointer
	Accelerometer* accelerometer;

	// I/O Controller pointer
	IOController* ioc;

private:
    // Update App
    void update();

    // Initialize App
    void initialize();

    // Deinitialize App
    void deInitialize();

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

    // App running flag
    bool running = false;
    int8_t exitCode = 0;
};

#endif