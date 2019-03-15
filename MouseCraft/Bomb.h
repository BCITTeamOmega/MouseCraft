#pragma once
#include "Contraption.h"
class Bomb :
	public Contraption
{
public:
	Bomb();
	~Bomb();

	bool use() override;
	void show() override;

	Entity* fieldEntity;
};

