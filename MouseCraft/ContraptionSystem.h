#pragma once

#include "Core/System.h"

class ContraptionSystem : public System
{
public:
	ContraptionSystem();
	~ContraptionSystem();
	void Update(float deltaSeconds) override;
};

