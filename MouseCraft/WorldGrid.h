#pragma once
#include <vector>
#include "Physics\PhysicsComponent.h"
#include <math.h>
#include "Core\Vector2D.h"

class WorldGrid
{
public:
	WorldGrid(int w, int h, int s);
	~WorldGrid();
	bool positionObject(Vector2D& pos);
	bool positionArea(Vector2D& p1, Vector2D& p2);
	void createObject(Vector2D& pos, PhysicsComponent* pcomp);
	void createArea(Vector2D& p1, Vector2D& p2, PhysicsComponent* pcomp);
	bool removeObject(float xPos, float yPos);
	bool removeArea(Vector2D* p1, Vector2D* p2);
	PhysicsComponent* objectAt(float xPos, float yPos);
	PhysicsComponent* objectAt(int xPos, int yPos);
	bool tileIsUp(float xPos, float yPos);
	bool tileIsUp(int xPos, int yPos);
	int gridWidth();
	int gridHeight();
private:
	std::vector<std::vector<bool>> baseGrid;
	std::vector<std::vector<PhysicsComponent*>> objectGrid;
	int scale;
};