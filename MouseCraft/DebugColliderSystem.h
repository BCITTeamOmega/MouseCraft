#pragma once

#include "Core/System.h"
#include "Core/ComponentManager.h"

class DebugColliderSystem : public System
{
public:
	DebugColliderSystem();
	~DebugColliderSystem();
	virtual void Update(float dt) override;
};

