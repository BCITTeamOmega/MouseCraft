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
};

