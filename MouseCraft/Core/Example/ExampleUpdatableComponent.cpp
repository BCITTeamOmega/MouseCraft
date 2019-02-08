#include "ExampleUpdatableComponent.h"

#include "../Entity.h"

ExampleUpdatableComponent::ExampleUpdatableComponent()
{
}


ExampleUpdatableComponent::~ExampleUpdatableComponent()
{
}

void ExampleUpdatableComponent::Update(float dt)
{
	counter += dt; 
	if (counter >= deactivateAfter)
	{
		GetEntity()->SetEnabled(false);
	}
}
