#pragma once
#include "../Event/ISubscriber.h"
#include "CContactListener.h"
#include "../Core/Vector2D.h"
#include <Box2D/Box2D.h>
#include <vector>
#include <utility>
#include "../Core/System.h"
#include "../Core/ComponentManager.h"
#include "PhysicsComponent.h"

constexpr auto FILTER1 = 0x0001;
constexpr auto FILTER2 = 0x0002;
constexpr auto FILTER3 = 0x0003;
constexpr auto FILTER4 = 0x0004;

class PhysicsManager : public System {
public:
	PhysicsManager();
	~PhysicsManager();
	void Update(float dt);
	void setOuterWalls(std::vector<std::pair<Vector2D, Vector2D>> walls);
	PhysicsComponent* createPlatform(float x, float y, float w, float h);
	PhysicsComponent* createPlayer(float x, float y, float w, float h, bool floor);
	PhysicsComponent* createKinematic(float x, float y, float w, float h, bool floor);
private:
	b2World *world;
	std::vector<b2Body*> players;
	CContactListener *cListener;
	void updateHeights(float delta);
	void checkCollisions();
};