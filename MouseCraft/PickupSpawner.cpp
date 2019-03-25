#include "PickupSpawner.h"

#include "Core/OmegaEngine.h"
#include "PickupFactory.h"
#include "MOUSECRAFT_ENUMS.h"
#include <glm/glm.hpp>

PickupSpawner::PickupSpawner()
{
}

PickupSpawner::~PickupSpawner()
{
}

void PickupSpawner::Update(float deltaTime)
{
	_counter += deltaTime;
	if (_counter > spawnDelay)
	{
		_counter -= spawnDelay;
		Spawn();
	}
}

void PickupSpawner::Spawn()
{
	int i = rand() % 3;
	float x = rand() % 100;
	float z = rand() % 70;

	Entity* entity;

	if (i == 0)
	{
		entity = PickupFactory::Instance().Create(PICKUPS::BATTERY, glm::vec3(x,0,z));
	}
	else if (i == 1)
	{
		entity = PickupFactory::Instance().Create(PICKUPS::SPRING, glm::vec3(x,0,z));
	}
	else 
	{
		entity = PickupFactory::Instance().Create(PICKUPS::SCREW, glm::vec3(x,0,z));
	}

	OmegaEngine::Instance().AddEntity(entity);

	std::cout << "Spawning new pickup at " << x << "," << z << std::endl;
}
