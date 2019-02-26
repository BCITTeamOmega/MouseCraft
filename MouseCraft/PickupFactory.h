#pragma once

#include "Pickup.h"
#include "MOUSECRAFT_ENUMS.h"
#include <glm/glm.hpp>
#include "Graphics/Model.h"

class PickupFactory
{
// singleton 
public:
	static PickupFactory& Instance()
	{
		static PickupFactory _instance;
		return _instance;
	}
	PickupFactory(PickupFactory const&) = delete;
	void operator=(PickupFactory const&) = delete;
private:
	PickupFactory();
	~PickupFactory();

// functions
public: 
	Entity* Create(PICKUPS type, glm::vec3 position);

// variables 
private:
	Model* _screwModel;
	Model* _springModel;
	Model* _batteryModel;
	Image* _texture; 
};

