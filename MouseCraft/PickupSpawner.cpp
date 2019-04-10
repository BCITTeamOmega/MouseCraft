#include "PickupSpawner.h"

#include "Core/OmegaEngine.h"
#include "PickupFactory.h"
#include "MOUSECRAFT_ENUMS.h"
#include <glm/glm.hpp>

PickupSpawner::PickupSpawner()
{
	// create list of floor position
	_grid = PhysicsManager::instance()->getGrid();

	for (int i = 0; i < _grid->gridWidth(); i++)
	{
		for (int j = 0; j < _grid->gridHeight(); j++)
		{
			if (!_grid->tileIsUp(i, j) && _grid->objectAt(i, j) == nullptr)
			{
				Vector2D pos = _grid->getScaledPosition(i, j);
				std::cout << "found free spot " << i << "," << j << std::endl;
				_floorPositions.push_back(pos);
			}
		}
	}
}

PickupSpawner::~PickupSpawner()
{
}

glm::vec3 PickupSpawner::GetFreePosition()
{
	int idx = rand() % _floorPositions.size();
	idx = rand() % _floorPositions.size();
	Vector2D pos = _floorPositions[idx];
	while (_grid->objectAt(pos.x, pos.y) != nullptr)
	{
		idx = rand() & _floorPositions.size();
		pos = _floorPositions[idx];
	}
	return glm::vec3(pos.x, 0.0f, pos.y);
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
	glm::vec3 pos = GetFreePosition();
	Entity* entity;

	if (i == 0)
	{
		entity = PickupFactory::Instance().Create(PICKUPS::BATTERY, pos);
	}
	else if (i == 1)
	{
		entity = PickupFactory::Instance().Create(PICKUPS::SPRING, pos);
	}
	else 
	{
		entity = PickupFactory::Instance().Create(PICKUPS::SCREW, pos);
	}

	OmegaEngine::Instance().AddEntity(entity);

	std::cout << "Spawning new pickup at " << pos.x << "," << pos.z << std::endl;
}
