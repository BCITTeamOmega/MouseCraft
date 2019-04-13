#pragma once
#include "../Core/Component.h"
#include "Color.h"
#include "../json.hpp"
#include "../Loading/PrefabLoader.h"
using json = nlohmann::json;

class Light : public Component {
public:
	enum class LightType {
		Directional = 0,
		Point = 1
	};
	Light();
	~Light();
	Color getColor();
	void setColor(Color c);
	float getConstantAttenuation();
	float getLinearAttenuation();
	float getQuadraticAttenuation();
	void setType(LightType type);
	LightType getType();
	void setAttenuation(float constant, float linear, float quadratic);
private:
	LightType _type;
	Color _color;
	float _constant;
	float _linear;
	float _quadratic;
	
	static Component* CreateFromJson(json json);
	static PrefabRegistrar reg;
};