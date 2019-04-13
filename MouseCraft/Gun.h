#pragma once
#include "Contraption.h"

class Mouse;

class Gun :
	public Contraption
{
public:
	Gun();
	~Gun();

	bool use(Mouse* m) override;
	void show() override;

private:
	float const BULLET_LIFETIME = 2.0f;
	float const BULLET_SPEED = 25.0f;
	float const BULLET_SIZE = 1.0f;	// chonk for testing

	static Component* Create(json json);
	static PrefabRegistrar reg;
};

