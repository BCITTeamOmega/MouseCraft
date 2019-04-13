#pragma once
#include "../Core/Component.h"
#include "Color.h"
class OutlineComponent : public Component {
public:
	OutlineComponent();
	~OutlineComponent();
	void setColor(Color c);
	Color getColor();
	void setWidth(float w);
	float getWidth();
private:
	Color _color;
	float _width;
};