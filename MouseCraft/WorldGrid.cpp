#include "WorldGrid.h"

//Make sure scale (s) divides into both the width and height otherwise the grid won't be as big as you intend
WorldGrid::WorldGrid(int w, int h, int s)
{
	scale = s;
	int gridW = w / scale;
	int gridH = h / scale;
	grid = std::vector<std::vector<PhysObjectType::PhysObjectType>>(gridW);

	//Their default values should be NOTHING since it's at index 0 of the enum (I think)
	for (int i = 0; i < gridW; i++)
		grid[i] = std::vector<PhysObjectType::PhysObjectType>(gridH);
}

WorldGrid::~WorldGrid()
{
	grid.clear();
}

//Corrects the positions to ensure they're in the grid then puts the object there
void WorldGrid::addObject(Vector2D& pos, PhysObjectType::PhysObjectType object)
{
	int xInd = round(pos.x / scale);
	int yInd = round(pos.y / scale);

	if (xInd < 0)
		xInd = 0;
	else if (xInd >= grid.size())
		xInd = grid.size() - 1;
	else if (yInd < 0)
		yInd = 0;
	else if (yInd >= grid[0].size())
		yInd = grid[0].size() - 1;

	grid[xInd][yInd] = object;

	pos.x = xInd * scale;
	pos.y = yInd * scale;
}

void WorldGrid::addArea(Vector2D& p1, Vector2D& p2, PhysObjectType::PhysObjectType type)
{
	int x1 = round(p1.x / scale);
	int x2 = round(p2.x / scale);
	int y1 = round(p1.y / scale);
	int y2 = round(p2.y / scale);

	if (x1 < 0)
		x1 = 0;
	else if (x1 >= grid.size())
		x1 = grid.size();
	else if (y1 < 0)
		y1 = 0;
	else if (y1 >= grid[0].size())
		y1 = grid[0].size() - 1;
	else if (x2 < 0)
		x2 = 0;
	else if (x2 >= grid.size())
		x2 = grid.size();
	else if (y2 < 0)
		y2 = 0;
	else if (y2 >= grid[0].size())
		y2 = grid[0].size() - 1;

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
			grid[x][y] = type;
		}
	}

	p1.x = x1 * scale;
	p1.y = y1 * scale;
	p2.x = x2 * scale;
	p2.y = y2 * scale;
}

bool WorldGrid::removeArea(Vector2D* p1, Vector2D* p2)
{
	int x1 = p1->x;
	int x2 = p2->x;
	int y1 = p1->y;
	int y2 = p2->y;

	if (x1 >= 0 && x1 < grid.size() && y1 >= 0 && y1 < grid[0].size()
		&& x2 >= 0 && x2 < grid.size() && y2 >= 0 && y2 < grid[0].size())
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
				grid[x][y] = replaceType(grid[x][y]); //replace it with whatever should be beneath it
			}
		}

		return true;
	}

	return false;
}

PhysObjectType::PhysObjectType WorldGrid::objectAt(float xPos, float yPos)
{
	int xInd = round(xPos);
	int yInd = round(yPos);

	if (xInd >= 0 && xInd < grid.size() && yInd >= 0 && yInd < grid[0].size())
		return grid[xInd][yInd];

	return PhysObjectType::NOTHING;
}

//Determines what type replaces another type when removed
PhysObjectType::PhysObjectType WorldGrid::replaceType(PhysObjectType::PhysObjectType type)
{
	switch (type)
	{
	case PhysObjectType::OBSTACLE_UP:
		return PhysObjectType::PLATFORM;
	case PhysObjectType::OBSTACLE_DOWN:
		return PhysObjectType::NOTHING;
	case PhysObjectType::PLATFORM:
		return PhysObjectType::NOTHING;
	case PhysObjectType::CONTRAPTION_UP:
		return PhysObjectType::PLATFORM;
	case PhysObjectType::CONTRAPTION_DOWN:
		return PhysObjectType::NOTHING;
	default:
		return PhysObjectType::NOTHING;
	}
}