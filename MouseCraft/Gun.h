#pragma once
#include "Contraption.h"
class Gun :
	public Contraption
{
public:
	Gun();
	~Gun();

	bool use() override;
	void show() override;

private:
	float const BULLET_LIFETIME = 2.0f;
	float const BULLET_SPEED = 25.0f;
	float const BULLET_SIZE = 1.0f;	// chonk for testing
};

