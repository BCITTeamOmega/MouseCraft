#pragma once

#include "Component.h"
#include "../EventManager/ISubscriber.h"

class UpdatableComponent : public Component, public ISubscriber
{
public:
	UpdatableComponent();
	~UpdatableComponent();
	virtual void Update(float deltaTime) = 0;
	virtual void Notify(EventName eventName, Param *params);
};

