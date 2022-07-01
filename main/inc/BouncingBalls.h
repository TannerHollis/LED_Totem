#ifndef BOUNCINGBALLS_H
#define BOUNCINGBALLS_H

#include <App.h>

#define DEFAULT_BALL_MAX 50

class BouncingBalls : public App
{
public :
	BouncingBalls(uint8_t id, 
		TotemController_t* tc,
		b2World* world) : App(id, tc)
	{
		this->world = world;
		ballDefaultCount = settings->getUInt("apps.bouncingBalls.defaultBallCount");
		ballBounciness = settings->getFloat("apps.bouncingBalls.ballBounciness");
		ballFriction = settings->getFloat("apps.bouncingBalls.ballFriction");
		ballDensity = settings->getFloat("apps.bouncingBalls.ballDensity");
		ballRadius = settings->getFloat("apps.bouncingBalls.ballRadius");
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
		for (uint8_t i = ball_count; i < end_count; i++) {
			if (ball_count == DEFAULT_BALL_MAX) {
				return;
			}
			else {
				// Define ball body
				b2BodyDef bodyDef;
				bodyDef.type = b2_dynamicBody;
				float x = randomFloat(0.0f, panel->getWidth());
				float y = randomFloat(0.0f, panel->getHeight());
				bodyDef.position.Set(x, y);
				balls[i] = world->CreateBody(&bodyDef);

				// Create random ball color
				ballColors[i] = Color::ColorLerpRainbow(0.0f, 1.0f, randomFloat(0.0f, 1.0f));

				// Define ball radius
				ballShape.m_radius = ballRadius;

				// Define the dynamic body fixture
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &ballShape;

				// Define ball density, friction and bounciness
				fixtureDef.density = ballDensity;
				fixtureDef.friction = ballFriction;
				fixtureDef.restitution = ballBounciness;

				// Add shape to body
				balls[i]->CreateFixture(&fixtureDef);

				ball_count++;
			}
		}
	}

	void removeBalls(uint8_t num)
	{
		int8_t end_count = ball_count - num;
		for (uint8_t i = ball_count - 1; i >= end_count; i--) {
			if (ball_count == 0) {
				return;
			}
			else {
				printf("Removing ball: %u\n", ball_count);
				world->DestroyBody(balls[i]);
				panel->clearPixel(balls[i]->GetPosition().x, balls[i]->GetPosition().y);
				ball_count--;
			}
		}
	}

private:
	void initializeRun()
	{
		printf("Initializing Bouncing Balls.\n");

		// Add edeges to world
		addEdges();

		// Add balls to world
		addBalls(ballDefaultCount);
	}

	void addEdges()
	{
		// Define ground body
		b2BodyDef bodyDef;
		bodyDef.position.Set(0.0f, 0.0f);

		// Create body factory
		groundBody = world->CreateBody(&bodyDef);

		// Calculate edge parameters
		float w = panel->getWidth();
		float h = panel->getHeight();
		float topH = 1.0f;
		float topW = w / 2.0f + 2 * topH;
		float posX = w / 2.0f;
		float posY = h / 2.0f;

		// Define ground walls
		groundBottom.SetAsBox(topW, topH, b2Vec2(posX, -topH / 2.0f), 0.0f);
		groundTop.SetAsBox(topW, topH, b2Vec2(posX, h + topH / 2.0f), 0.0f);
		groundLeft.SetAsBox(topH, topW, b2Vec2(-topH / 2.0f, posY), 0.0f);
		groundRight.SetAsBox(topH, topW, b2Vec2(w + topH / 2.0f, posY), 0.0f);

		// Add ground walls to world
		groundBody->CreateFixture(&groundBottom, 0.0f);
		groundBody->CreateFixture(&groundTop, 0.0f);
		groundBody->CreateFixture(&groundLeft, 0.0f);
		groundBody->CreateFixture(&groundRight, 0.0f);
	}

	void deInitializeRun()
	{
		printf("De-Initializing Bouncing Balls.\n");
		b2Body* bodies = world->GetBodyList();
		while (bodies)
		{
			world->DestroyBody(bodies);
			bodies = bodies->GetNext();
		}
	}

	// Pre-process update
	void preUpdate()
	{
		// Clear LEDs at positions
		for (uint32_t i = 0; i < ball_count; i++) {
			panel->clearPixel(balls[i]->GetPosition().x, balls[i]->GetPosition().y);
		}

		world->SetGravity(b2Vec2(100.0f * accelerometer->aX(), 100.0f * accelerometer->aY()));
	}

	// Process update
	void processUpdate()
	{
		world->Step(getFrameTime(), velocityIterations, positionIterations);
	}

	// Post-process update
	void postUpdate()
	{
		// Clear LEDs at positions
		for (uint32_t i = 0; i < ball_count; i++) {
			panel->setPixel(balls[i]->GetPosition().x, balls[i]->GetPosition().y, ballColors[i]);
		}

		// Display LEDs
		panel->display();
	}

	// Process Inputs
	void processInputs()
	{
		if (ioc->isNextLongHeld()) {
			stop(1);
		}
		else if (ioc->isPreviousLongHeld()) {
			stop(-1);
		}
		else if (ioc->isNextShortPressed()) {
			addBalls(1);
		}
		else if (ioc->isPreviousShortPressed()) {
			removeBalls(1);
		}
	}

	// Box2D objects
	b2World* world;
	b2Body* groundBody;
	b2PolygonShape groundBottom;
	b2PolygonShape groundLeft;
	b2PolygonShape groundRight;
	b2PolygonShape groundTop;
	b2Body* balls[DEFAULT_BALL_MAX];
	Color ballColors[DEFAULT_BALL_MAX];
	b2CircleShape ballShape;

	// Box2D simulation vars
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// Simulation vars
	int8_t ball_count = 0;

	// Simulation vars initialized with settings
	float ballDefaultCount;
	float ballBounciness;
	float ballFriction;
	float ballDensity;
	float ballRadius;
};

#endif