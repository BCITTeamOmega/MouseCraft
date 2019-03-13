#pragma once
#include <vector>

class WorldGrid
{
public:
	WorldGrid(int w, int h);
private:
	std::vector<std::vector<int>> grid;
};