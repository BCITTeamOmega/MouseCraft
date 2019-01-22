#pragma once
#include "EventManager/ISubscriber.h"
#include <Box2D/Box2D.h>

class PhysicsManager : public ISubscriber {
public:
	PhysicsManager();
	~PhysicsManager();

	void Update(const float delta);
private:
};