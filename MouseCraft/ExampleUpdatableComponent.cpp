#include "ExampleUpdatableComponent.h"

#include "Core/Entity.h"

ExampleUpdatableComponent::ExampleUpdatableComponent()
{
}


ExampleUpdatableComponent::~ExampleUpdatableComponent()
{
}

void ExampleUpdatableComponent::update(float dt)
{
	counter += dt; 
	if (counter >= deactivateAfter)
	{
		getEntity()->setEnabled(false);
	}
}
