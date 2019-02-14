#pragma once

#include "../Component.h"
#include "glm/glm.hpp"

// Demonstration of a component that a system uses. 
// Moves the entity towards the specified direction.
class ExampleComponent : public Component
{
public:
	ExampleComponent() : velocity(rand() % 10, rand() % 10, rand() % 10) {};
	~ExampleComponent() {};
	glm::vec3 velocity;
};

