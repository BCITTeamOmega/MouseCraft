#pragma once

#include "Core/System.h"
#include "Core/ComponentManager.h"
#include "DebugColliderComponent.h"

class DebugColliderSystem : public System
{
public:
	DebugColliderSystem();
	~DebugColliderSystem();
	bool CheckCollision(DebugColliderComponent* left, DebugColliderComponent* right);
	virtual void Update(float dt) override;
};

