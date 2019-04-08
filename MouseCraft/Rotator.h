#pragma once

#include <glm/glm.hpp>
#include "Core/UpdatableComponent.h"

class Rotator : public UpdatableComponent
{
public:
	Rotator();
	~Rotator();
	virtual void Update(float deltaTime);
	glm::vec3 rotationSpeed;
};

