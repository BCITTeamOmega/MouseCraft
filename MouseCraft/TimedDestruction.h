#pragma once

#include "Core/UpdatableComponent.h"

class TimedDestruction : public UpdatableComponent
{
public:
	TimedDestruction();
	~TimedDestruction();
	virtual void Update(float deltaTime) override;
	float delay = 5.0f;		// time till destruction
private:
	float _counter;
};

