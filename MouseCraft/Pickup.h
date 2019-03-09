#pragma once

#include "Core/Component.h"
#include "MOUSECRAFT_ENUMS.h"
#include "./Physics/PhysicsComponent.h"

class Pickup : public Component, public Observer<PhysicsComponent*>
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
};

