#pragma once

#include "Core/Entity.h"
#include "Core/Component.h"
#include "MOUSECRAFT_ENUMS.h"
#include "DebugColliderComponent.h"


class Contraption : public Component
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
	DebugColliderComponent* _collider;

};

