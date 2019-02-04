#pragma once
#include "EventManager/ISubscriber.h"
#include "CContactListener.h"
#include "Vector2D.h"
#include <Box2D/Box2D.h>
#include <vector>
#include <utility>

class PhysicsManager : public ISubscriber {
public:
	PhysicsManager();
	~PhysicsManager();
	void Update(const float delta);
	void setOuterWalls(std::vector<std::pair<Vector2D, Vector2D>> walls);
private:
	b2World *floorWorld, *upperWorld;
	b2Body **players;
	CContactListener* cListener;
	void updateHeights();
	void checkCollisions();
};