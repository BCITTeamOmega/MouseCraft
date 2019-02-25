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

#pragma region Awful macros
constexpr auto FALL_VELOCITY = 10;
constexpr auto JUMP_VELOCITY = 10;
constexpr auto Z_UPPER = 10.0;
constexpr auto Z_THRESHOLD = 6.0;
constexpr auto Z_LOWER = 1.0;

constexpr auto WALL_CATEGORY = 0x0001;
constexpr auto PLATFORM_CATEGORY = 0x0002;
constexpr auto OBSTACLE_DOWN_CATEGORY = 0x0004;
constexpr auto OBSTACLE_UP_CATEGORY = 0x0008;
constexpr auto MOUSE_DOWN_CATEGORY = 0x0010;
constexpr auto MOUSE_UP_CATEGORY = 0x0020;
constexpr auto CAT_DOWN_CATEGORY = 0x0040;
constexpr auto CAT_UP_CATEGORY = 0x0080;
constexpr auto CONTRAPTION_DOWN_CATEGORY = 0x0100;
constexpr auto CONTRAPTION_UP_CATEGORY = 0x0200;
constexpr auto PART_CATEGORY = 0x0400;

constexpr auto PART_MASK = 0x0014;
constexpr auto CONTRAPTION_UP_MASK = 0x0269;
constexpr auto CONTRAPTION_DOWN_MASK = 0x0157;
constexpr auto CAT_UP_MASK = 0x022B;
constexpr auto CAT_DOWN_MASK = 0x0117;
constexpr auto MOUSE_UP_MASK = 0x028B;
constexpr auto MOUSE_DOWN_MASK = 0x0547;
constexpr auto OBSTACLE_UP_MASK = 0x02AA;
constexpr auto OBSTACLE_DOWN_MASK = 0x0554;
constexpr auto PLATFORM_MASK = 0x01F8;
constexpr auto WALL_MASK = 0x03F0;
#pragma endregion

enum PhysObjectType
{
	PART,
	CONTRAPTION_UP,
	CONTRAPTION_DOWN,
	CAT_UP,
	CAT_DOWN,
	MOUSE_UP,
	MOUSE_DOWN,
	OBSTACLE_UP,
	OBSTACLE_DOWN,
	PLATFORM
};

class PhysicsManager : public System {
public:
	PhysicsManager();
	~PhysicsManager();
	void Update(float dt);
	void setOuterWalls(std::vector<std::pair<Vector2D, Vector2D>> walls);
	PhysicsComponent* createObject(float x, float y, float w, float h, float r, PhysObjectType t);
private:
	b2World *world;
	std::vector<b2Body*> players;
	CContactListener *cListener;
	void updateHeights(float delta);
	void checkCollisions();
};