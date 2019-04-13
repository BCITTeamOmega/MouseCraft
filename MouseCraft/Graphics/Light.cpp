#include "Light.h"
#include <string>

using std::string;

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

#pragma region prefab support 

Component* Light::CreateFromJson(json json)
{
	auto c = ComponentManager<Light>::Instance().Create<Light>();

	// parse type
	if (json.find("lightType") != json.end()) {
		c->_type = json["lightType"].get<string>() == "Directional"
			? Light::LightType::Directional
			: Light::LightType::Point;
	}

	// parse color 
	if (json.find("color") != json.end())
	{
		c->_color = Color(
			json["color"]["r"].get<double>(),
			json["color"]["g"].get<double>(),
			json["color"]["b"].get<double>());
	}

	// parse other data
	if (json.find("attenuation") != json.end())
	{
		c->_constant = json["attenuation"]["constant"].get<double>();
		c->_linear = json["attenuation"]["linear"].get<double>();
		c->_quadratic = json["attenuation"]["quadratic"].get<double>();
	}

	return c;
}

PrefabRegistrar Light::reg("Light", &Light::CreateFromJson);