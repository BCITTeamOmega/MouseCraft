#pragma once
#include "Contraption.h"
class Gun :
	public Contraption
{
public:
	Gun();
	~Gun();

	void use() override;
	void show() override;

private:
	float const range = 10.0;
};

