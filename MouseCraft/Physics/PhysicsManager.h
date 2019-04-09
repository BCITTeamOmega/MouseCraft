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
constexpr auto GRAVITY = -12;
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
constexpr auto BALL_UP_CATEGORY = 0x2000;
constexpr auto BALL_DOWN_CATEGORY = 0x4000;
constexpr auto COLLISIONLESS_CATEGORY = 0x8000;

constexpr auto PART_MASK = 0x0000;
constexpr auto CONTRAPTION_UP_MASK = 0x22A9;
constexpr auto CONTRAPTION_DOWN_MASK = 0x4157;
constexpr auto CAT_UP_MASK = 0x1209;
constexpr auto CAT_DOWN_MASK = 0x0907;
constexpr auto MOUSE_UP_MASK = 0x2209;
constexpr auto MOUSE_DOWN_MASK = 0x4107;
constexpr auto OBSTACLE_UP_MASK = 0x32A9;
constexpr auto OBSTACLE_DOWN_MASK = 0x4957;
constexpr auto PLATFORM_MASK = 0x4954;
constexpr auto WALL_MASK = 0x7BFC;
constexpr auto PROJECTILE_DOWN_MASK = 0x0047;
constexpr auto PROJECTILE_UP_MASK = 0x0089;
constexpr auto BALL_UP_MASK = 0x0229;
constexpr auto BALL_DOWN_MASK = 0x0117;
constexpr auto COLLISIONLESS_MASK = 0x0000;
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
	WorldGrid* getGrid();
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