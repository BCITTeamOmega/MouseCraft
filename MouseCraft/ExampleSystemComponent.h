#pragma once

#include "Core/System.h"

class ExampleSystemComponent : public System
{
public:
	ExampleSystemComponent();
	~ExampleSystemComponent();
	void update(float dt) override;
};

