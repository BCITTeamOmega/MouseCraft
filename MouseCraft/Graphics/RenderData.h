#pragma once
#include "Model.h"
class RenderData {
public:
	RenderData(Model* m, float nx, float ny, float nz) : _model(m), _x(nx), _y(ny), _z(nz) {}
	Model* getModel() { return _model; }
	float getX() { return _x; }
	float getY() { return _y; }
	float getZ() { return _z; }
private:
	Model* _model;
	float _x;
	float _y;
	float _z;
};