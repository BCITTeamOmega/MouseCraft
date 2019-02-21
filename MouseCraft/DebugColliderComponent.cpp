#include "DebugColliderComponent.h"

#include <iostream>


DebugColliderComponent::DebugColliderComponent()
{
	OnCollide.Attach(this);
}

DebugColliderComponent::~DebugColliderComponent()
{
	OnCollide.Detach(this);
}

void DebugColliderComponent::Publish(DebugColliderComponent *, DebugColliderComponent *)
{
	std::cout << "COLLIDED" << std::endl;
}

