#ifndef WATERSIMULATION_H
#define WATERSIMULATION_H

#include <App.h>

#define PARTICLE_RADIUS 0.5f
#define PARTICLE_DAMPING 0.2f

class WaterSimulation : public App
{
public :
    WaterSimulation(uint8_t id,
		LEDPanel* panel,
		Accelerometer* accelerometer,
		IOController* ioc,
		b2World* world) : App(id, panel, accelerometer, ioc)
	{
		this->world = world;
	}
	~WaterSimulation()
	{
		// Do nothing.
	}

private:
	void initializeRun()
	{
		printf("Initializing Water Simulation.\n");
		
		// Add edges to world
		addEdges();

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

	void addParticles()
	{
		float x = panel->getWidth();
		float y = panel->getHeight();

		// Define particle system
		b2ParticleSystemDef particleSystemDef;
		particleSystem = world->CreateParticleSystem(&particleSystemDef);
		particleSystem->SetRadius(PARTICLE_RADIUS);
		particleSystem->SetDamping(PARTICLE_DAMPING);

		// Define particle group shape
		particleShape.SetAsBox(x / 4.0f, y / 4.0f, b2Vec2(x * 0.5f, y * 0.5f), 0.0f);

		// Define particle group definition
		b2ParticleGroupDef particleGroupDef;
		particleGroupDef.flags = 0; // Water particles
		particleGroupDef.shape = &particleShape;
		particleGroup = particleSystem->CreateParticleGroup(particleGroupDef);

		printf("Particles created: %u\n", particleGroup->GetParticleCount());
	}

	void deInitializeRun()
	{
		printf("De-Initializing Water Simulation.\n");
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
	}

	// Pre-process update
	void preUpdate()
	{
		// Clear LEDs at positions
		b2Vec2* positions = particleSystem->GetPositionBuffer();
		for (uint32_t i = 0; i < particleSystem->GetParticleCount(); i++) {
			panel->clearPixel(positions[i].x, positions[i].y);
		}

		// Set gravity
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
		// Set LED at positions
		b2Vec2* positions = particleSystem->GetPositionBuffer();
		for (uint32_t i = 0; i < particleSystem->GetParticleCount(); i++) {
			panel->setPixel(positions[i].x, positions[i].y, Color::ColorLerpRainbow(0.0f, panel->getHeight(), positions[i].y));
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
	b2ParticleSystem* particleSystem;
	b2PolygonShape particleShape;
	b2ParticleGroup* particleGroup;

	// Box2D simulation vars
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
};

#endif