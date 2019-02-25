#pragma once
#include "Contraption.h"
class Coil :
	public Contraption
{
public:
	Coil();
	~Coil();

	void use() override;
	void show() override;
};

