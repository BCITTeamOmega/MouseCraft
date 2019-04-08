#pragma once

#include "Core/Component.h"
#include "MOUSECRAFT_ENUMS.h"
#include "./Physics/PhysicsComponent.h"
#include "Event/Handler.h"
#include "Rotator.h"

class Pickup : public Component
{
// functions 
public:
	Pickup();
	~Pickup();
	
	virtual void OnInitialized() override;
	void Grab();
	void Drop();
	
// variables 
public:
	PICKUPS type;
private:
	PhysicsComponent* _physics;
	Rotator* _rotator;
};

