#pragma once

#include "Core/UpdatableComponent.h"
#include "Physics/PhysicsManager.h"

class PickupSpawner : public UpdatableComponent
{
public:
	PickupSpawner();
	~PickupSpawner();
	virtual void Update(float deltaTime) override;
	void Spawn();

public:
	float spawnDelay = 4.0f;
private:
	glm::vec3 GetFreePosition();
	float _counter = 0.0f;
	std::vector<glm::ivec2> _floorPositions;
	WorldGrid* _grid;
};

