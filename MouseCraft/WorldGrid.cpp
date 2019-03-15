#include "WorldGrid.h"

WorldGrid::WorldGrid(int w, int h)
{
	grid = std::vector<std::vector<PhysObjectType::PhysObjectType>>(w);

	//Their default values should be NOTHING since it's at index 0 of the enum (I think)
	for (int i = 0; i < w; i++)
		grid[i] = std::vector<PhysObjectType::PhysObjectType>(h);
}

bool WorldGrid::addObject(float xPos, float yPos, PhysObjectType::PhysObjectType object)
{
	int xInd = round(xPos);
	int yInd = round(yPos);

	if (xInd >= 0 && xInd < grid.size() && yInd >= 0 && yInd < grid[0].size())
	{
		grid[xInd][yInd] = object;
		return true;
	}
	else
	{
		return false;
	}
}

bool WorldGrid::addArea(Vector2D* p1, Vector2D* p2, PhysObjectType::PhysObjectType type)
{
	int x1 = p1->x;
	int x2 = p2->x;
	int y1 = p1->y;
	int y2 = p2->y;

	if (x1 >= 0 && x1 < grid.size() && y1 >= 0 && y1 < grid[0].size()
		&& x2 >= 0 && x2 < grid.size() && y2 >= 0 && y2 < grid[0].size())
	{
		//These fix the points if the user input them in the wrong order or something
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

		for (int x = x1; x <= x2; x++)
		{
			for (int y = y1; y <= y2; y++)
			{
				grid[x][y] = type;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}