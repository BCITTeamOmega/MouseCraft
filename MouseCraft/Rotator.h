#pragma once

#include <glm/glm.hpp>
#include "Core/UpdatableComponent.h"

#include "Loading/PrefabLoader.h"
#include "json.hpp"
using json = nlohmann::json;

class Rotator : public UpdatableComponent
{
public:
	Rotator();
	~Rotator();
	virtual void Update(float deltaTime);
	glm::vec3 rotationSpeed;

	/* TEMPLATE
	{
		"type": "Rotator",
		"speed": [3.14, 3.14, 3.14]
	}
	*/
	static Component* Create(json json);
	static PrefabRegistrar reg;
};

