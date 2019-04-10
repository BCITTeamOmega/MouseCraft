#include "OutlineComponent.h"

OutlineComponent::OutlineComponent() : _color(Color(0.0f, 0.0f, 0.0f)), _width(1.0f) {

}

OutlineComponent::~OutlineComponent() {

}

void OutlineComponent::setColor(Color c) {
	_color = c;
}

Color OutlineComponent::getColor() {
	return _color;
}

void OutlineComponent::setWidth(float w) {
	_width = w;
}

float OutlineComponent::getWidth() {
	return _width;
}