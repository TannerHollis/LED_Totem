#ifndef BOUNCINGBALL_H
#define BOUNCINGBALL_H

#include <App.h>

class BouncingBall : public App
{
public :
	BouncingBall(uint8_t id, 
		LEDPanel* panel,
		Accelerometer* accelerometer,
		b2World* world) : App(id, panel, accelerometer)
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
		printf("Initializing Bouncingball.\n");
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

		// Define ball body
		ballBodyDef.type = b2_dynamicBody;
		ballBodyDef.position.Set(0.0f, 5.0f);
		ballBodyDef.angle = 1.0f;
		ball = world->CreateBody(&ballBodyDef);

		// Define ball radius
		ballShape.m_radius = 0.5f;

		// Define the dynamic body fixture
		ballFixtureDef.shape = &ballShape;

		// Define ball density, friction and bounciness
		ballFixtureDef.density = 1.0f;
		ballFixtureDef.friction = 0.3f;
		ballFixtureDef.restitution = 0.5f;

		// Add shape to body
		ball->CreateFixture(&ballFixtureDef);
	}

	void deInitializeRun()
	{
		printf("De-Initializing Bouncingball.\n");
		b2Body* bodies = world->GetBodyList();
		while (bodies) 
		{
			world->DestroyBody(bodies);
			bodies = bodies->GetNext();
		}
	}

	// Pre-process update
	void preUpdate() {
		b2Vec2 position = ball->GetPosition();
		printf("X: %4.2f, Y: %4.2f\n", position.x, position.y);
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
	b2Body* ball;
	b2CircleShape ballShape;
	b2FixtureDef ballFixtureDef;

	// Box2D simulation vars
	float32 timeStep = 1 / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
};

#endif