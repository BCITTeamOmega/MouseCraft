#pragma once

#include "../System.h"

class ExampleSystem : public System
{
public:
	ExampleSystem();
	~ExampleSystem();
	void Update(float dt) override;
};

