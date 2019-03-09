#pragma once
#include "../Core/Component.h"
#include "../Core/Transform.h"
#include "Model.h"
#include "Color.h"
#include "../Loading/PrefabLoader.h"
#include "../json.hpp"
using json = nlohmann::json;

class Renderable : public Component {
public:
	Renderable();
	Model* getModel();
	void setModel(Model& model);
	Transform getTransform();
	Color getColor();
	void setColor(Color color);
private:
	Model* _model;
	Color _color;

	static Component* CreateFromJson(json json);
	static PrefabRegistrar reg;
};