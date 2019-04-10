#pragma once

#include "Core/Entity.h"
#include "Core/Component.h"
#include "Core/OmegaEngine.h"
#include "MOUSECRAFT_ENUMS.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/PhysObjectType.h"
#include "HealthComponent.h"
#include "PlayerComponent.h"
#include "DamageOnCollision.h"
#include "TimedDestruction.h"

class Mouse;

class Contraption : public Component, public Observer<PhysicsComponent*>
{
public:
	Contraption();
	~Contraption();

	virtual void OnInitialized() override;
	virtual void Update(float dt);
	virtual bool use(Mouse* m);
	virtual void show();
	void Grab();
	void Drop();

public:
	CONTRAPTIONS type;

protected:
	Mouse* mouse;
};

