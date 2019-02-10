#pragma once

#include "../System.h"

class ExampleSystemComponent : public System
{
public:
	ExampleSystemComponent();
	~ExampleSystemComponent();
	void Update(float dt) override;
};

