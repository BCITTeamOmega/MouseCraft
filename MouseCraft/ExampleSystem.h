#pragma once

#include "Core/System.h"

class ExampleSystem : public System
{
public:
	ExampleSystem();
	~ExampleSystem();
	void update(float dt) override;
};

