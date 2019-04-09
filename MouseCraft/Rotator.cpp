#include "Rotator.h"

#include "Core/Entity.h"

Rotator::Rotator()
{
}

Rotator::~Rotator()
{
}

void Rotator::Update(float deltaTime)
{
	GetEntity()->transform.rotate(rotationSpeed * deltaTime);
}
