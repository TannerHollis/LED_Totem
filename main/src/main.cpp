#include <Apps.h>
#include <stdio.h>
#include <signal.h>

#define CFG_FILE_NAME "settings.cfg"

TotemController_t tc;

void handler(int signal) {
	printf("Exiting on code %u\n", signal);
	tc.running = false;
}

int main(int argc, char** argv)
{
	// Establish signal handler
	struct sigaction sa;
	sa.sa_handler = handler;
	sigaction(SIGINT, &sa, NULL);

	// Initialize settings
	Setting settings(CFG_FILE_NAME);

	// Initialize I/O Controller
	IOController ioc = IOController(&settings);

    // Initialize LED Panel
    LEDPanel panel = LEDPanel(&settings);
	
	// Initialize MPU Unit
	Accelerometer mpu6050 = Accelerometer(&settings);

	// Create Totem Controller for readability
	tc.settings = &settings;
	tc.panel = &panel;
	tc.ioc = &ioc;
	tc.mpu = &mpu6050;
	tc.running = true;
	
	// Create gravity and gravity
	b2Vec2 gravity(0.0f, 0.0f);
	b2World world(gravity);

	// Define apps
	App *apps[] = { new BouncingBall(0, &tc, &world),
					new BouncingBalls(1, &tc, &world),
					new WaterSimulation(2, &tc, &world),
					new WaterWheel(3, &tc, &world)};

	// Initialize and run each app
	for (App* app : apps) {
        app->loop();
		if (!tc.running)
			break;
	}

	panel.clearPanel();
	panel.display();
	return 0;
}
