#ifndef BOUNCINGBALL_H
#define BOUNCINGBALL_H

#include <App.h>

#define BALL_POS_HISTORY 125
#define BALL_POS_HISTORY_MAX 200
#define BALL_VELOCITY_MAX 1000.0f
#define DEFAULT_BALL_BOUNCINESS 0.7f

class BouncingBall : public App
{
public :
	BouncingBall(uint8_t id, 
		LEDPanel* panel,
		Accelerometer* accelerometer,
		IOController* ioc,
		b2World* world) : App(id, panel, accelerometer, ioc)
	{
		this->world = world;
	}
	~BouncingBall()
	{
		// Do nothing.
	}

private:
	void initializeRun()
	{
		printf("Initializing Bouncing Ball.\n");
		
		// Add edges to world
		addEdges();

		// Define ball body
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(panel->getWidth() / 2.0f, panel->getHeight() / 2.0f);
		ball = world->CreateBody(&bodyDef);

		// Define ball radius
		ballShape.m_radius = 0.5f;

		// Define the dynamic body fixture
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &ballShape;

		// Define ball density, friction and bounciness
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.1f;
		fixtureDef.restitution = 0.8f;

		// Add shape to body
		ball->CreateFixture(&fixtureDef);
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
		float topW = w / 2.0f + 2.0f * topH;
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
		printf("De-Initializing Bouncing Ball.\n");
		b2Body* bodies = world->GetBodyList();
		while (bodies) 
		{
			world->DestroyBody(bodies);
			bodies = bodies->GetNext();
		}

		// Reset simulation variables
		ballPositionHead = 0;
		ballPositionTail = 0;
	}

	// Pre-process update
	void preUpdate()
	{
		if (ballPositionHead - ballPositionTail >= 0) {
			for (uint16_t i = ballPositionTail; i < ballPositionHead; i++) {
				panel->clearPixel(ballPositions[i].x, ballPositions[i].y);
			}
		}
		else {
			for (uint16_t i = ballPositionTail; i < BALL_POS_HISTORY_MAX; i++) {
				panel->clearPixel(ballPositions[i].x, ballPositions[i].y);
			}
			for (uint16_t i = 0; i < ballPositionHead; i++) {
				panel->clearPixel(ballPositions[i].x, ballPositions[i].y);
			}
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
		// Update head
		if (ballPositionHead == BALL_POS_HISTORY_MAX - 1) {
			ballPositionHead = 0;
		}
		else {
			ballPositionHead++;
		}

		// Update tail
		uint16_t diff = (ballPositionHead - ballPositionTail > 0) ? ballPositionHead - ballPositionTail : ballPositionHead + BALL_POS_HISTORY_MAX - ballPositionTail;
		if (diff == BALL_POS_HISTORY) {
			if (ballPositionTail == BALL_POS_HISTORY_MAX - 1) {
				ballPositionTail = 0;
			}
			else {
				ballPositionTail++;
			}
		}

		// Add new position
		ballPositions[ballPositionHead] = ball->GetPosition();
		ballColors[ballPositionHead] = Color::ColorLerpRainbow(0.0f, BALL_VELOCITY_MAX, ball->GetLinearVelocity().LengthSquared());

		// Set pixels and fade tail
		if (ballPositionHead - ballPositionTail >= 0) {
			float scale = 1.0f / BALL_POS_HISTORY;
			for (uint16_t i = ballPositionTail; i < ballPositionHead; i++) {
				panel->setPixel(ballPositions[i].x, ballPositions[i].y, ballColors[i].Scale(scale));
				scale += 1.0f / BALL_POS_HISTORY;
			}
		}
		else {
			float scale = 1.0f / BALL_POS_HISTORY;
			for (uint16_t i = ballPositionTail; i < BALL_POS_HISTORY_MAX; i++) {
				panel->setPixel(ballPositions[i].x, ballPositions[i].y, ballColors[i].Scale(scale));
				scale += 1.0f / BALL_POS_HISTORY;
			}
			for (uint16_t i = 0; i < ballPositionHead; i++) {
				panel->setPixel(ballPositions[i].x, ballPositions[i].y, ballColors[i].Scale(scale));
				scale += 1.0f / BALL_POS_HISTORY;
			}
		}
		
		// Display LEDs
		panel->display();
	}

	// Process Inputs
	void processInputs()
	{
		if (ioc->isButtonLongHeld(INPUT_BTTN_NEXT)) {
			stop(1);
		}
		else if (ioc->isButtonLongHeld(INPUT_BTTN_PREVIOUS)) {
			stop(-1);
		}
	}

	// Box2D objects
	b2World* world;
	b2Body* groundBody;
	b2PolygonShape groundBottom;
	b2PolygonShape groundLeft;
	b2PolygonShape groundRight;
	b2PolygonShape groundTop;
	b2Body* ball;
	b2CircleShape ballShape;

	// Box2D simulation vars
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// Simulation vars
	b2Vec2 ballPositions[BALL_POS_HISTORY_MAX];
	Color ballColors[BALL_POS_HISTORY_MAX];
	uint16_t ballPositionHead = 0;
	uint16_t ballPositionTail = 0;
};

#endif