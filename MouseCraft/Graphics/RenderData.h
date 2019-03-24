#pragma once
#include "Model.h"
#include "Color.h"
#include <glm/glm.hpp>
class RenderData {
public:
	RenderData(Model* m, glm::mat4 transform, Color c = Color(1.0f, 1.0f, 1.0f))
		:_transform(transform), _color(c) {
		_model = new Model(*m);
	}
	Model* getModel() { return _model; }
	glm::mat4 getTransform() { return _transform; }
	Color getColor() { return _color; }
private:
	Model* _model;
	glm::mat4 _transform;
	Color _color;
};