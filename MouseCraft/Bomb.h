#pragma once
#include "Contraption.h"
class Bomb :
	public Contraption
{
public:
	Bomb();
	~Bomb();

	void use() override;
	void show() override;

	Entity* fieldEntity;
};

