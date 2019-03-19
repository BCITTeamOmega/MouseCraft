#pragma once
#include "Contraption.h"
class Overcharge :
	public Contraption
{
public:
	Overcharge();
	~Overcharge();

	bool use() override;
	void show() override;
};

