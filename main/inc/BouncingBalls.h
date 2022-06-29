#ifndef BOUNCINGBALLS_H
#define BOUNCINGBALLS_H

#include <App.h>

#define DEFAULT_BALL_COUNT 3
#define DEFAULT_BALL_MAX 50

class BouncingBalls : public App
{
public :
	BouncingBalls(uint8_t id, 
		LEDPanel* panel,
		Accelerometer* accelerometer,
		b2World* world) : App(id, panel, accelerometer)
	{
		this->world = world;
	}
	~BouncingBalls()
	{
		// Do nothing.
	}

	float randomFloat(float min, float max)
	{
		return min + (float)(rand()) / ((float)RAND_MAX / (max - min));
	}

	void addBalls(uint8_t num)
	{
		int8_t end_count = ball_count + num;
		for (int8_t i = ball_count; i < end_count; i++) {
			if (ball_count == DEFAULT_BALL_MAX - 1) {
				return;
			}
			else {
				// Define ball body
				ballBodyDef.type = b2_dynamicBody;
				float x = randomFloat(-5.0f, 5.0f);
				float y = randomFloat(0.0f, 10.0f);
				ballBodyDef.position.Set(x, y);
				ballBodyDef.angle = 0.0f;
				balls[i] = world->CreateBody(&ballBodyDef);

				// Define ball radius
				ballShape.m_radius = 0.5f;

				// Define the dynamic body fixture
				ballFixtureDef.shape = &ballShape;

				// Define ball density, friction and bounciness
				ballFixtureDef.density = 1.0f;
				ballFixtureDef.friction = 0.3f;
				ballFixtureDef.restitution = 0.5f;

				// Add shape to body
				balls[i]->CreateFixture(&ballFixtureDef);
				ball_count++;
			}
		}
	}

	void removeBalls(uint8_t num)
	{
		int8_t end_count = ball_count - num;
		for (int8_t i = ball_count; i > end_count; i--) {
			if (ball_count == -1) {
				return;
			}
			else {
				world->DestroyBody(balls[i]);
				ball_count--;
			}
		}
	}

private:
	void initializeRun()
	{
		printf("Initializing Bouncingballs.\n");
		// Define ground body
		groundBodyDef.position.Set(0.0f, 0.0f);

		// Create body factory
		groundBody = world->CreateBody(&groundBodyDef);

		// Define ground walls
		groundBottom.SetAsBox(7.0f, 1.0f, b2Vec2(0.0f, -1.0f), 0.0f);
		groundTop.SetAsBox(7.0f, 1.0f, b2Vec2(0.0f, 11.0f), 0.0f);
		groundLeft.SetAsBox(1.0f, 7.0f, b2Vec2(-6.0f, 5.0f), 0.0f);
		groundRight.SetAsBox(1.0f, 7.0f, b2Vec2(6.0f, 5.0f), 0.0f);

		// Add ground walls to world
		groundBody->CreateFixture(&groundBottom, 0.0f);
		groundBody->CreateFixture(&groundTop, 0.0f);
		groundBody->CreateFixture(&groundLeft, 0.0f);
		groundBody->CreateFixture(&groundRight, 0.0f);

		// Add balls to world
		addBalls(DEFAULT_BALL_COUNT);
	}

	void deInitializeRun()
	{
		printf("De-Initializing Bouncingballs.\n");
		b2Body* bodies = world->GetBodyList();
		while (bodies)
		{
			world->DestroyBody(bodies);
			bodies = bodies->GetNext();
		}
		ball_count = 0;
	}

	// Pre-process update
	void preUpdate() {
		b2Vec2 position;
		for (uint8_t i = 0; i < ball_count; i++) {
			position = balls[i]->GetPosition();
			printf("Ball: %u, X: %4.2f, Y: %4.2f\n", i, position.x, position.y);
		}
	}

	// Process update
	void processUpdate()
	{
		world->Step(timeStep, velocityIterations, positionIterations);
	}

	// Post-process update
	void postUpdate() {
		// Do nothing.
	}

	// Box2D objects
	b2World* world;
	b2BodyDef groundBodyDef;
	b2Body* groundBody;
	b2PolygonShape groundBottom;
	b2PolygonShape groundLeft;
	b2PolygonShape groundRight;
	b2PolygonShape groundTop;
	b2BodyDef ballBodyDef;
	b2Body* balls[DEFAULT_BALL_MAX];
	b2CircleShape ballShape;
	b2FixtureDef ballFixtureDef;

	// Box2D simulation vars
	float32 timeStep = 1 / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// Simulation vars
	int8_t ball_count = 0;
};

#endif