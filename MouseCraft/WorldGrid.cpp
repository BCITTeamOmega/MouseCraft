#include "WorldGrid.h"

//Make sure scale (s) divides into both the width and height otherwise the grid won't be as big as you intend
WorldGrid::WorldGrid(int w, int h, int s)
{
	scale = s;
	int gridW = w / scale;
	int gridH = h / scale;
	baseGrid = std::vector<std::vector<bool>>(gridW);
	objectGrid = std::vector<std::vector<PhysicsComponent*>>(gridW);

	//Their default values should be NOTHING since it's at index 0 of the enum (I think)
	for (int i = 0; i < gridW; i++)
	{
		baseGrid[i] = std::vector<bool>(gridH);
		objectGrid[i] = std::vector<PhysicsComponent*>(gridH);
	}
}

WorldGrid::~WorldGrid()
{
	baseGrid.clear();
	objectGrid.clear();
}

//Corrects the positions to ensure they're in the grid
//Returns false if there is something already there
bool WorldGrid::positionObject(Vector2D& pos)
{
	int xInd = round(pos.x / scale);
	int yInd = round(pos.y / scale);

	if (xInd < 0)
		xInd = 0;
	else if (xInd >= baseGrid.size())
		xInd = baseGrid.size() - 1;
	else if (yInd < 0)
		yInd = 0;
	else if (yInd >= baseGrid[0].size())
		yInd = baseGrid[0].size() - 1;

	if (objectGrid[xInd][yInd] != nullptr)
		return false;

	pos.x = xInd * scale;
	pos.y = yInd * scale;

	return true;
}

//Returns false if there is something already there
bool WorldGrid::positionArea(Vector2D& p1, Vector2D& p2)
{
	int x1 = round(p1.x / scale);
	int x2 = round(p2.x / scale);
	int y1 = round(p1.y / scale);
	int y2 = round(p2.y / scale);

	if (x1 < 0)
		x1 = 0;
	else if (x1 >= baseGrid.size())
		x1 = baseGrid.size();
	else if (y1 < 0)
		y1 = 0;
	else if (y1 >= baseGrid[0].size())
		y1 = baseGrid[0].size() - 1;
	else if (x2 < 0)
		x2 = 0;
	else if (x2 >= baseGrid.size())
		x2 = baseGrid.size();
	else if (y2 < 0)
		y2 = 0;
	else if (y2 >= baseGrid[0].size())
		y2 = baseGrid[0].size() - 1;

	//Fix the points if the user input them in the wrong order or something
	if (x1 > x2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if(y2 > y1)
	{
		int temp = y2;
		y2 = y1;
		y1 = temp;
	}

	for (int x = x1; x < x2; x++)
	{
		for (int y = y1; y < y2; y++)
		{
			if (objectGrid[x][y] != nullptr)
				return false;
		}
	}

	p1.x = x1 * scale;
	p1.y = y1 * scale;
	p2.x = x2 * scale;
	p2.y = y2 * scale;

	return true;
}

//Corrects the positions to ensure they're in the grid then puts the object there
//Returns false if there is something already there
void WorldGrid::createObject(Vector2D& pos, PhysicsComponent* pcomp)
{
	if (objectGrid[pos.x][pos.y] != nullptr)
		objectGrid[pos.x][pos.y] = pcomp;
}

//Returns false if there is something already there
void WorldGrid::createArea(Vector2D& p1, Vector2D& p2, PhysicsComponent* pcomp)
{
	for (int x = p1.x; x < p2.x; x++)
	{
		for (int y = p1.y; y < p2.y; y++)
		{
			if (objectGrid[x][y] != nullptr)
				objectGrid[x][y] = pcomp;
		}
	}
}

bool WorldGrid::removeObject(float xPos, float yPos)
{
	int xInd = round(xPos / scale);
	int yInd = round(yPos / scale);

	if (xInd >= 0 && xInd < baseGrid.size() && yInd >= 0 && yInd < baseGrid[0].size())
		objectGrid[xInd][yInd] = nullptr;

	return false;
}

bool WorldGrid::removeArea(Vector2D* p1, Vector2D* p2)
{
	int x1 = p1->x;
	int x2 = p2->x;
	int y1 = p1->y;
	int y2 = p2->y;

	if (x1 >= 0 && x1 < baseGrid.size() && y1 >= 0 && y1 < baseGrid[0].size()
		&& x2 >= 0 && x2 < baseGrid.size() && y2 >= 0 && y2 < baseGrid[0].size())
	{
		//Fix the points if the user input them in the wrong order or something
		if (x1 > x2)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
		}

		if (y2 > y1)
		{
			int temp = y2;
			y2 = y1;
			y1 = temp;
		}

		for (int x = x1; x <= x2; x++)
		{
			for (int y = y1; y <= y2; y++)
			{
				objectGrid[x][y] = nullptr;
			}
		}

		return true;
	}

	return false;
}

PhysicsComponent* WorldGrid::objectAt(float xPos, float yPos)
{
	int xInd = round(xPos / scale);
	int yInd = round(yPos / scale);

	if (xInd >= 0 && xInd < baseGrid.size() && yInd >= 0 && yInd < baseGrid[0].size())
		return objectGrid[xInd][yInd];

	return nullptr;
}

PhysicsComponent* WorldGrid::objectAt(int xPos, int yPos)
{
	if (xPos >= 0 && xPos < baseGrid.size() && yPos >= 0 && yPos < baseGrid[0].size())
		return objectGrid[xPos][yPos];
}

bool WorldGrid::tileIsUp(float xPos, float yPos)
{
	int xInd = round(xPos / scale);
	int yInd = round(yPos / scale);

	if (xInd >= 0 && xInd < baseGrid.size() && yInd >= 0 && yInd < baseGrid[0].size())
		return baseGrid[xInd][yInd];

	return false;
}

bool WorldGrid::tileIsUp(int xPos, int yPos)
{
	if (xPos >= 0 && xPos < baseGrid.size() && yPos >= 0 && yPos < baseGrid[0].size())
		return baseGrid[xPos][yPos];
}

int WorldGrid::gridWidth()
{
	return baseGrid.size();
}

int WorldGrid::gridHeight()
{
	return baseGrid[0].size();
}