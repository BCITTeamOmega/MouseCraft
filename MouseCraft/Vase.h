#pragma once

#include "Obstacle.h"

class Vase : public Obstacle
{
public:
	Vase();
	~Vase();
	virtual void HitByCat(Vector2D dir) override {
		std::cout << "Cat hit the vase. Not implemented yet." << std::endl;
	};
	virtual void DestroyedByMouse() override {};
};

