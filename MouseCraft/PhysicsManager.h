#pragma once
#include "Event/ISubscriber.h"
#include "CContactListener.h"
#include "Vector2D.h"
#include <Box2D/Box2D.h>
#include <vector>
#include <utility>
#include "Core/System.h"
#include "Core/ComponentManager.h"
#include "PhysicsComponent.h"

class PhysicsManager : public System, public ISubscriber {
public:
	PhysicsManager();
	~PhysicsManager();
	void Update(float dt);
	void setOuterWalls(std::vector<std::pair<Vector2D, Vector2D>> walls);
	void createPlayer(float x, float y, float w, float h, bool floor);
	void createKinematic(float x, float y, float w, float h, bool floor);
private:
	b2World *floorWorld, *upperWorld;
	b2Body **players;
	CContactListener* cListener;
	void updateHeights(float delta);
	void checkCollisions();
};