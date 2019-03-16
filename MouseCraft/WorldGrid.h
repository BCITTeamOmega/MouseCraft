#pragma once
#include <vector>
#include "Physics\PhysObjectType.h"
#include <math.h>
#include "Core\Vector2D.h"

class WorldGrid
{
public:
	WorldGrid(int w, int h);
	~WorldGrid();
	bool addObject(float xPos, float yPos, PhysObjectType::PhysObjectType type);
	bool addArea(Vector2D* p1, Vector2D* p2, PhysObjectType::PhysObjectType type);
	bool removeArea(Vector2D* p1, Vector2D* p2);
	PhysObjectType::PhysObjectType objectAt(float xPos, float yPos);
private:
	std::vector<std::vector<PhysObjectType::PhysObjectType>> grid;

	PhysObjectType::PhysObjectType replaceType(PhysObjectType::PhysObjectType type);
};