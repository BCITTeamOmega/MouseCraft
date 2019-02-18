#include "Renderable.h"
#include "../Core/Entity.h"

Model* Renderable::getModel() {
	return _model;
}

void Renderable::setModel(Model& model) {
	_model = &model;
}

Transform Renderable::getTransform() {
	Entity* e = GetEntity();
	return e->transform;
}

Color Renderable::getColor() {
	return _color;
}

void Renderable::setColor(Color color) {
	_color = color;
}