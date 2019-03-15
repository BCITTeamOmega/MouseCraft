#pragma once
#include <vector>
#include "Physics\PhysObjectType.h"
#include <math.h>
#include "Core\Vector2D.h"

class WorldGrid
{
public:
	WorldGrid(int w, int h);
	bool addObject(float xPos, float yPos, PhysObjectType::PhysObjectType type);
	bool addArea(Vector2D* p1, Vector2D* p2, PhysObjectType::PhysObjectType type);
private:
	std::vector<std::vector<PhysObjectType::PhysObjectType>> grid;
};