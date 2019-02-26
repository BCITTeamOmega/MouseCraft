#pragma once
#include "Contraption.h"
class Swords :
	public Contraption
{
public:
	Swords();
	~Swords();

	void use() override;
	void show() override;
};

