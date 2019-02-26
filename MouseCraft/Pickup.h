#pragma once

#include "Core/Component.h"
#include "MOUSECRAFT_ENUMS.h"
#include "DebugColliderComponent.h"

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
	DebugColliderComponent* _collider;
};

