#include "Light.h"

Light::Light() : _color(Color(1.0f, 1.0f, 1.0f)), _constant(1.0f), _linear(0.25f), _quadratic(0.025f) {
}

Light::~Light() {
}

Color Light::getColor() {
	return _color;
}

void Light::setColor(Color c) {
	_color = c;
}

float Light::getConstantAttenuation() {
	return _constant;
}

float Light::getLinearAttenuation() {
	return _linear;
}

float Light::getQuadraticAttenuation() {
	return _quadratic;
}

void Light::setAttenuation(float constant, float linear, float quadratic) {
	_constant = constant;
	_linear = linear;
	_quadratic = quadratic;
}

Light::LightType Light::getType() {
	return _type;
}

void Light::setType(Light::LightType type) {
	_type = type;
}