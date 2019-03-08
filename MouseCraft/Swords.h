#pragma once
#include "Contraption.h"
#include "Physics/PhysObjectType.h"

class Swords :
	public Contraption
{
public:
	Swords();
	~Swords();

	void use() override;
	void show() override;
};

