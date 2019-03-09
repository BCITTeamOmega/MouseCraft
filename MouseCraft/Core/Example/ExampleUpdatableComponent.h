#pragma once

#include "../UpdatableComponent.h"

// Demonstration of a systemless component. 
// Deactivates the entity after specified seconds. 
class ExampleUpdatableComponent : public UpdatableComponent
{
public:
	ExampleUpdatableComponent();
	~ExampleUpdatableComponent();
	void Update(float dt) override;
	float deactivateAfter = 10;
private:
	float counter;
};

