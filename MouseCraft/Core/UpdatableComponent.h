#pragma once

#include "Component.h"
#include "../EventManager/ISubscriber.h"

class UpdatableComponent : public Component, public ISubscriber
{
public:
	UpdatableComponent();
	~UpdatableComponent();
	void onInitialized() override;
	virtual void update(float deltaTime) = 0;
	virtual void Notify(EventName eventName, Param *params);
};

