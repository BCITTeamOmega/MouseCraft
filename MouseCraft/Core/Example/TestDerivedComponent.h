#pragma once

#include "TestComponent.h"

// DO NOT LOOK AT THIS COMPONENT FOR USAGE
// This component is used for testing, it will look very strange.
class TestDerivedComponent : public TestComponent
{
public:
	TestDerivedComponent(Entity* target = nullptr);
	~TestDerivedComponent();
	void update(float dt) override {};
};

