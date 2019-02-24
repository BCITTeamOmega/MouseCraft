#pragma once
#include "Contraption.h"
class Platform :
	public Contraption
{
public:
	Platform();
	~Platform();

	void use() override;
	void show() override;
};

