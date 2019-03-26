#pragma once

#include "Obstacle.h"

class Obstruction : public Obstacle
{
public:
	Obstruction();
	~Obstruction();
	virtual void HitByCat(Vector2D dir) override {
		std::cout << "Cat hit an obstruction (book/box). Not implemented yet." << std::endl;
	};
	virtual void DestroyedByMouse() override {};
};

