#pragma once
#include "Contraption.h"
class Platform :
	public Contraption
{
public:
	Platform();
	~Platform();

	bool use() override;
	void show() override;

	Entity* fieldEntity;
};

