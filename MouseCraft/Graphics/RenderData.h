#pragma once
#include "Model.h"
class RenderData {
public:
	RenderData(Model m, float nx, float ny, float nz) : model(m), x(nx), y(ny), z(nz) {}
private:
	Model model;
	float x;
	float y;
	float z;
};