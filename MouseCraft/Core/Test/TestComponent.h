#pragma once

#include "../UpdatableComponent.h"
#include "../Component.h"
#include "../Entity.h"

// DO NOT LOOK AT THIS COMPONENT FOR USAGE
// This component is used for testing, it will look very strange.
class TestComponent : public UpdatableComponent
{
public:
	TestComponent(Entity* target);
	~TestComponent();
	void Update(float dt) override;

private: 
	int _frame = 0;
	Entity* target;
};

