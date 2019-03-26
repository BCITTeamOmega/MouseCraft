#include "UIRenderable.h"
#include "ModelGen.h"
#include "../Core/Entity.h"

using std::string;

UIRenderable::UIRenderable() {
	_model = ModelGen::makeQuad(ModelGen::Axis::Z, 1.0f, 1.0f);
}

UIRenderable::~UIRenderable() {

}

void UIRenderable::setSize(float width, float height) {
	string* tmp = _model->getTexture();
	delete _model;
	_model = ModelGen::makeQuad(ModelGen::Axis::Z, width, height);
	_model->setTexture(tmp);
	_width = width;
	_height = height;
}

void UIRenderable::setTexture(std::string* path) {
	_model->setTexture(path);
}

float UIRenderable::getWidth() {
	return _width;
}

float UIRenderable::getHeight() {
	return _height;
}

Transform UIRenderable::getTransform() {
	Entity* e = GetEntity();
	if (e != nullptr) {
		return e->transform;
	}
}

Color UIRenderable::getColor() {
	return _color;
}

void UIRenderable::setColor(Color color) {
	_color = color;
}

Model* UIRenderable::getModel() {
	return _model;
}