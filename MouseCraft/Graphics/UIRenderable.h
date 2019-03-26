#pragma once
#include "../Core/Component.h"
#include "Model.h"
#include "Color.h"
#include "../Core/Transform.h"
#include <string>
class UIRenderable : public Component {
public:
	UIRenderable();
	~UIRenderable();
	void setSize(float width, float height);
	void setTexture(std::string* path);
	float getWidth();
	float getHeight();
	Model* getModel();
	Transform getTransform();
	Color getColor();
	void setColor(Color c);
private:
	float _width;
	float _height;
	Model* _model;
	Color _color;
};