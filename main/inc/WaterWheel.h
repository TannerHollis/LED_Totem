#ifndef WATERWHEEL_H
#define WATERWHEEL_H

#include <App.h>

class WaterWheel : public App
{
public :
	WaterWheel(uint8_t id,
		TotemController_t* tc,
		b2World* world) : App(id, tc)
	{
		this->world = world;
		waterSizeWidth = settings->getFloat("apps.waterWheel.water.size.width");
		waterSizeHeight = settings->getFloat("apps.waterWheel.water.size.height");
		waterParticleFlag = settings->getUInt("apps.waterWheel.water.particleFlag");
		waterParticleRadius = settings->getFloat("apps.waterWheel.water.particleRadius");
		waterParticleDamping = settings->getFloat("apps.waterWheel.water.particleDamping");
		waterParticleDensity = settings->getFloat("apps.waterWheel.water.particleDensity");
		waterParticleMaxVelocity = settings->getFloat("apps.waterWheel.water.particleMaxVelocity");
		waterWheelLength = settings->getFloat("apps.waterWheel.wheel.length");
		waterWheelMaxTorque = settings->getFloat("apps.waterWheel.wheel.maxTorque");
		waterWheelMaxSpeed = settings->getFloat("apps.waterWheel.wheel.maxSpeed");
	}
	~WaterWheel()
	{
		// Do nothing.
	}

private:
	void initializeRun()
	{
		printf("Initializing Water Wheel.\n");
		
		// Add edges to world
		addEdges();

		// Add water wheel
		addWaterWheel();

		// Add particle system
		addParticles();
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

	void addWaterWheel()
	{
		// Define water wheel shape
		float wheelLength = waterWheelLength;
		float wheelWidth = 0.2f;

		// Define water wheel shape
		waterWheelShape.SetAsBox(wheelWidth / 2.0f, wheelLength / 2.0f);

		// Define water wheel center shape
		waterWheelCenterShape.m_radius = 0.1f;

		// Place water wheel center
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(panel->getWidth() / 2.0f, panel->getHeight() / 2.0f);
		waterWheelCenter = world->CreateBody(&bodyDef);
		waterWheelCenter->CreateFixture(&waterWheelCenterShape, 1.0f);

		// Place water wheel
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(panel->getWidth() / 2.0f, panel->getHeight() / 2.0f);
		waterWheel = world->CreateBody(&bodyDef);
		waterWheel->CreateFixture(&waterWheelShape, 1.0f);

		// Define motor parameters
		b2RevoluteJointDef revoluteJointDef;
		revoluteJointDef.bodyA = waterWheel;
		revoluteJointDef.bodyB = waterWheelCenter;
		revoluteJointDef.localAnchorA.Set(0, 0);
		revoluteJointDef.localAnchorB.Set(0, 0);
		revoluteJointDef.maxMotorTorque = waterWheelMaxTorque;
		revoluteJointDef.motorSpeed = waterWheelSpeed;
		revoluteJointDef.enableMotor = true;
		revoluteJointDef.collideConnected = false;
		waterWheelJoint = (b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef);
	}

	void addParticles()
	{
		float x = panel->getWidth();
		float y = panel->getHeight();

		// Define particle system
		b2ParticleSystemDef particleSystemDef;
		particleSystem = world->CreateParticleSystem(&particleSystemDef);
		particleSystem->SetRadius(waterParticleRadius);
		particleSystem->SetDamping(waterParticleDamping);
		particleSystem->SetDensity(waterParticleDensity);

		// Define particle group shape
		b2ParticleGroupDef particleGroupDef;
		particleShape.SetAsBox(waterSizeWidth / 2.0f, waterSizeHeight / 2.0f, b2Vec2(x * 0.5f, y * 0.5f), 0.0f);
		particleGroupDef.flags = waterParticleFlag; // Water particles
		particleGroupDef.shape = &particleShape;
		particleGroup = particleSystem->CreateParticleGroup(particleGroupDef);

		printf("\tParticles created: %u\n", particleGroup->GetParticleCount());
	}

	void deInitializeRun()
	{
		printf("De-Initializing Water Wheel.\n");
		b2Body* bodies = world->GetBodyList();
		while (bodies)
		{
			world->DestroyBody(bodies);
			bodies = bodies->GetNext();
		}

		b2ParticleSystem* particleSystems = world->GetParticleSystemList();
		while (particleSystems)
		{
			world->DestroyParticleSystem(particleSystems);
			particleSystems = particleSystems->GetNext();
		}

		b2Joint* joints = world->GetJointList();
		while (joints)
		{
			world->DestroyJoint(joints);
			joints = joints->GetNext();
		}
	}

	// Pre-process update
	void preUpdate()
	{
		// Clear LEDs at positions
		b2Vec2* positions = particleSystem->GetPositionBuffer();
		for (uint32_t i = 0; i < particleSystem->GetParticleCount(); i++) {
			panel->clearPixel(positions[i].x, positions[i].y);
		}

		// Set water wheel LEDs
		b2Vec2 v0, v1;
		b2Vec2 offset = waterWheel->GetPosition();
		float angle = waterWheel->GetAngle();
		for (uint8_t i = 0; i < waterWheelShape.GetVertexCount() - 1; i++) {
			v0 = transformVector(waterWheelShape.GetVertex(i), angle, offset);
			v1 = transformVector(waterWheelShape.GetVertex(i + 1), angle, offset);
			panel->drawLine(v0.x, v0.y, v1.x, v1.y, Color(0.0f, 0.0f, 0.0f));
		}
		v0 = transformVector(waterWheelShape.GetVertex(0), angle, offset);
		panel->drawLine(v0.x, v0.y, v1.x, v1.y, Color(0.0f, 0.0f, 0.0f));

		// Set gravity
		world->SetGravity(b2Vec2(100.0f * accelerometer->aX(), 100.0f * accelerometer->aY()));

		// Set motor speed
		waterWheelJoint->SetMotorSpeed(waterWheelSpeed);
	}

	// Process update
	void processUpdate()
	{
		world->Step(getFrameTime(), velocityIterations, positionIterations);
	}

	// Post-process update
	void postUpdate()
	{
		// Set LED at positions
		b2Vec2* positions = particleSystem->GetPositionBuffer();
		b2Vec2* velocities = particleSystem->GetVelocityBuffer();
		for (uint32_t i = 0; i < particleSystem->GetParticleCount(); i++) {
			panel->setPixel(positions[i].x, positions[i].y, Color::ColorLerpRainbow(0.0f, waterParticleMaxVelocity, velocities[i].LengthSquared()));
		}

		// Set water wheel LEDs
		b2Vec2 v0, v1;
		b2Vec2 offset = waterWheel->GetPosition();
		float angle = waterWheel->GetAngle();
		Color white(0.2f, 0.2f, 0.2f);
		for (uint8_t i = 0; i < waterWheelShape.GetVertexCount() - 1; i++) {
			v0 = transformVector(waterWheelShape.GetVertex(i), angle, offset);
			v1 = transformVector(waterWheelShape.GetVertex(i + 1), angle, offset);
			panel->drawLine(v0.x, v0.y, v1.x, v1.y, white);
		}
		v0 = transformVector(waterWheelShape.GetVertex(0), angle, offset);
		panel->drawLine(v0.x, v0.y, v1.x, v1.y, white);

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
		waterWheelSpeed = (ioc->getEncoderPositionLinear() * 2.0f - 1.0f) * waterWheelMaxSpeed;
	}

	b2Vec2 transformVector(b2Vec2 in, float angle, b2Vec2 offset)
	{
		float x = cos(angle) * in.x - sin(angle) * in.y + offset.x;
		float y = sin(angle) * in.x + cos(angle) * in.y + offset.y;
		return b2Vec2(x, y);
	}

	// Box2D objects
	b2World* world;
	b2Body* groundBody;
	b2PolygonShape groundBottom;
	b2PolygonShape groundLeft;
	b2PolygonShape groundRight;
	b2PolygonShape groundTop;
	b2ParticleSystem* particleSystem;
	b2PolygonShape particleShape;
	b2ParticleGroup* particleGroup;
	b2Body* waterWheel;
	b2PolygonShape waterWheelShape;
	b2Body* waterWheelCenter;
	b2CircleShape waterWheelCenterShape;
	b2RevoluteJoint* waterWheelJoint;

	// Box2D simulation vars
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// Simulation vars
	float waterWheelSpeed = 0.0f; // rad/sec

	// Simulation vars initialized with settings
	float waterSizeWidth;
	float waterSizeHeight;
	uint8_t waterParticleFlag;
	float waterParticleRadius;
	float waterParticleDamping;
	float waterParticleDensity;
	float waterParticleMaxVelocity;
	float waterWheelLength;
	float waterWheelMaxTorque;
	float waterWheelMaxSpeed;
};

#endif