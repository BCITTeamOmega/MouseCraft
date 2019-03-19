#pragma once
#include <vector>
#include "Physics\PhysObjectType.h"
#include <math.h>
#include "Core\Vector2D.h"

class WorldGrid
{
public:
	WorldGrid(int w, int h, int scale);
	~WorldGrid();
	void addObject(Vector2D& pos, PhysObjectType::PhysObjectType type);
	void addArea(Vector2D& p1, Vector2D& p2, PhysObjectType::PhysObjectType type);
	bool removeArea(Vector2D* p1, Vector2D* p2);
	PhysObjectType::PhysObjectType objectAt(float xPos, float yPos);
private:
	std::vector<std::vector<PhysObjectType::PhysObjectType>> grid;
	int scale;

	PhysObjectType::PhysObjectType replaceType(PhysObjectType::PhysObjectType type);
};