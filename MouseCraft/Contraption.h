#pragma once

#include "Core/Entity.h"
#include "Core/Component.h"
#include "MOUSECRAFT_ENUMS.h"
#include "Physics/PhysicsComponent.h"
#include "Event/Observer.h"


class Contraption : public Component, public Observer<PhysicsComponent*>
{
public:
	Contraption();
	~Contraption();

	virtual void OnInitialized() override;
	virtual void use();
	virtual void show();
	void Grab();
	void Drop();

public:
	CONTRAPTIONS type;
private:

};

