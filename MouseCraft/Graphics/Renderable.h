#pragma once
#include "../Core/Component.h"
#include "../Core/Transform.h"
#include "Model.h"
#include "Color.h"
class Renderable : public Component {
public:
	Model* getModel();
	void setModel(Model& model);
	Transform getTransform();
	Color getColor();
	void setColor(Color color);
private:
	Model* _model;
	Color _color;
};