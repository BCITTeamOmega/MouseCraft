#pragma once

#include "Core/Component.h"
#include "MOUSECRAFT_ENUMS.h"
#include "./Physics/PhysicsComponent.h"
#include "Event/Handler.h"
#include "Loading/PrefabLoader.h"
#include "WorldGrid.h"
#include "Rotator.h"

#include "json.hpp"
using json = nlohmann::json;

class Pickup : public Component
{
// functions 
public:
	Pickup();
	~Pickup();
	
	virtual void OnInitialized() override;
	void Grab();
	void Drop(glm::vec3);
	
// variables 
public:
	PICKUPS type;
private:
	PhysicsComponent* _physics;
	Rotator* _rotator;	
	
	static Component* Create(json json);
	static PrefabRegistrar reg;
};

