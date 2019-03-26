#pragma once

#include "Core/UpdatableComponent.h"

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
	float _counter = 0.0f;
};

