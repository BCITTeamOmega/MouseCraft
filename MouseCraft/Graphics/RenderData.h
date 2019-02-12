#pragma once
#include "Model.h"
#include "Color.h"
class RenderData {
public:
	RenderData(Model* m, float nx, float ny, float nz, Color c = Color(1.0f, 1.0f, 1.0f))
		: _model(m), _x(nx), _y(ny), _z(nz), _color(c) {}
	Model* getModel() { return _model; }
	float getX() { return _x; }
	float getY() { return _y; }
	float getZ() { return _z; }
	Color getColor() { return _color; }
private:
	Model* _model;
	float _x;
	float _y;
	float _z;
	Color _color;
};