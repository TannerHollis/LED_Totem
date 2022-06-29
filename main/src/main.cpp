/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
* Copyright (c) 2013 Google, Inc.
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <Accelerometer.h>
#include <Apps.h>
#include <lgpio.h>
#include <stdio.h>

// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
// There are no graphics for this example. Box2D is meant to be used
// with your rendering engine in your game engine.
int main(int argc, char** argv)
{
    // Initialize LED Panel
    LEDPanel panel = LEDPanel(1, 1, 16, 16);
	
	// Initialize MPU Unit
	Accelerometer mpu6050 = Accelerometer();
	mpu6050.setAccelerometerAutoScale(0.75f);

	// Create gravity
	b2Vec2 gravity(0.0f, -10.0f);

	b2World world(gravity);

	// Define apps
	App *apps[] = { new BouncingBall(0, &panel, &mpu6050, &world),
					new BouncingBalls(1, &panel, &mpu6050, &world)};

	// Define start time
	double timeStart, frameTimeStart;

	apps[0]->initialize();

	// Initialize and run each app
	for (App* app : apps) {
		app->initialize();
		timeStart = lguTime();
		while(lguTime() - timeStart < 5.0f) {
			frameTimeStart = lguTime();
			app->update();
			while (lguTime() - frameTimeStart < 1.0f / 60.0f) {
				// Do nothing.
			}
		}
		app->deInitialize();
	}

	Color_t color = {128, 128, 0};
	panel.fillPanel(color);

	float timeStep = 1.0f / 60.0f;
	timeStart = lguTime();
    for (float i = 0.0f; i < 1.0f; i += timeStep) {
		printf("Time elapsed: %4.4f\n", lguTime() - timeStart);
		panel.display();
    }

	timeStart = lguTime();
	while (lguTime() - timeStart < 2.0f) {
		printf("X: %4.4f, Y: %4.4f, Z: %4.4f, Temp(F): %4.4f\n", mpu6050.aX(), mpu6050.aY(), mpu6050.aZ(), mpu6050.tempF());
		lguSleep(0.01f);
	}

	return 0;
}
