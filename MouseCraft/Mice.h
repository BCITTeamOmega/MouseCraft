#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif // !GLM_ENABLE_EXPERIMENTAL

#include <iomanip>
#include <glm/gtx/string_cast.hpp>
#include "Pickup.h"
#include "Contraption.h"
#include "ContraptionFactory.h"
#include "Core/OmegaEngine.h"
#include "Core/Entity.h"
#include "Core/Component.h"
#include "Core/UpdatableComponent.h"
#include "Event/EventManager.h"
#include "Event/Observer.h"
#include "Event/Handler.h"
#include "Input/InputSystem.h"
#include "Physics/PhysicsComponent.h"
#include "HealthComponent.h"
#include "PlayerComponent.h"

class Mice : public UpdatableComponent
{
public:
	Mice();
	~Mice();

	virtual void OnInitialized() override;
	virtual void Update(float deltaTime);
	virtual void Notify(EventName eventName, Param *params) override;
	virtual void OnCollision(PhysicsComponent* e);
	virtual void OnHit(PhysicsComponent* e);
	virtual void OnDeath();	
	void addItem(Pickup* item);
	void dropItem();
	void use(Contraption* item);
	void combine(Pickup *material);

public:
	float speed = 10.0f;
	bool downed = false;
	Handler<Mice, PhysicsComponent*> HandleOnCollide;
	Handler<Mice, PhysicsComponent*> HandleOnHit;
	Handler<Mice> HandleOnDeath;

private:
	int player = 0;
	float moveX;
	float moveY;
	float aimX;
	float aimY;
	bool shoot;
	bool drop;
	Pickup* baseItem;
	Contraption* newItem;
};

