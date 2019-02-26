#pragma once
#include "Contraption.h"
class Overcharge :
	public Contraption
{
public:
	Overcharge();
	~Overcharge();

	void use() override;
	void show() override;
};

