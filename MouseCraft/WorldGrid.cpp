#include "WorldGrid.h"

WorldGrid::WorldGrid(int w, int h)
{
	grid = std::vector<std::vector<int>>(w);

	for (int i = 0; i < w; i++)
		grid[i] = std::vector<int>(h);
}