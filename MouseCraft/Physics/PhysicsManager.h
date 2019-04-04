#pragma once
#include "../Event/ISubscriber.h"
#include "CContactListener.h"
#include "../Core/Vector2D.h"
#include <Box2D/Box2D.h>
#include <vector>
#include <utility>
#include <set>
#include "../Core/System.h"
#include "../Core/ComponentManager.h"
#include "PhysicsComponent.h"
#include <glm/glm.hpp>
#include "PhysObjectType.h"
#include "../Core/Entity.h"
#include "AreaQueryCallback.h"
#include "RayQueryCallback.h"
#include "../Util/CpuProfiler.h"
#include "../WorldGrid.h"

#pragma region Awful macros
constexpr auto FALL_VELOCITY = 5;
constexpr auto JUMP_VELOCITY = 5;
constexpr auto Z_UPPER = 5.0;
constexpr auto Z_THRESHOLD = 3.0;
constexpr auto Z_LOWER = 0.5;

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
constexpr auto PROJECTILE_DOWN_CATEGORY = 0x0800;
constexpr auto PROJECTILE_UP_CATEGORY = 0x1000;

constexpr auto PART_MASK = 0x0010;
constexpr auto CONTRAPTION_UP_MASK = 0x02A9;
constexpr auto CONTRAPTION_DOWN_MASK = 0x0157;
constexpr auto CAT_UP_MASK = 0x0229;
constexpr auto CAT_DOWN_MASK = 0x0117;
constexpr auto MOUSE_UP_MASK = 0x02A9;
constexpr auto MOUSE_DOWN_MASK = 0x0557;
constexpr auto OBSTACLE_UP_MASK = 0x02A9;
constexpr auto OBSTACLE_DOWN_MASK = 0x0157;
constexpr auto PLATFORM_MASK = 0x0154;
constexpr auto WALL_MASK = 0x03FC;
constexpr auto PROJECTILE_DOWN_MASK = 0x0047;
constexpr auto PROJECTILE_UP_MASK = 0x0089;
#pragma endregion

class PhysicsComponent;
class CContactListener;

class PhysicsManager : public System
{
public:
	static PhysicsManager* instance();
	static void destroy();
	void Update(float dt);
	void setupGrid(int w, int h, int scale);
	PhysicsComponent* createObject(float x, float y, float w, float h, float r, PhysObjectType::PhysObjectType t);
	PhysicsComponent* createGridObject(float x, float y, int w, int h, PhysObjectType::PhysObjectType t);
	std::vector<PhysicsComponent*> areaCheck(PhysicsComponent* checkedBy, Vector2D* p1, Vector2D* p2);
	std::vector<PhysicsComponent*> areaCheck(PhysicsComponent* checkedBy, std::set<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2);
	PhysicsComponent* rayCheck(PhysicsComponent* checkedBy, Vector2D* p1, Vector2D* p2, Vector2D& hit);
	PhysicsComponent* rayCheck(PhysicsComponent* checkedBy, std::set<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2, Vector2D& hit);
private:
	static PhysicsManager* pmInstance;
	CpuProfiler profiler;
	b2World *world;
	CContactListener *cListener;
	WorldGrid* grid;

	PhysicsManager();
	~PhysicsManager();
	void updateHeights(float delta);
	void checkCollisions();
};