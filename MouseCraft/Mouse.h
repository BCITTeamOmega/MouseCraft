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
#include "Sound\SoundComponent.h"
#include "PickupFactory.h"
#include "PlayerComponent.h"

constexpr auto MOUSE_JUMP_DIST = 10;

class Mouse : public UpdatableComponent
{
public:
	Mouse();
	~Mouse();

	virtual void OnInitialized() override;
	virtual void Update(float deltaTime);
	virtual void Notify(EventName eventName, Param *params) override;
	virtual void OnCollision(PhysicsComponent* e);
	virtual void OnHit(PhysicsComponent* e);
	virtual void OnBounce(PhysicsComponent* e);
	virtual void OnDeath();
	virtual void OnRevived();
	void addItem(Pickup* item);
	void dropItem();
	void use(Contraption* item);
	void combine(Pickup *material);
	void revive(PhysicsComponent* mouse);

public:
	float speed = 10.0f;
	bool downed = false;
	Handler<Mouse, PhysicsComponent*> HandleOnCollide;
	Handler<Mouse, PhysicsComponent*> HandleOnHit;
	Handler<Mouse, PhysicsComponent*> HandleOnBounce;
	Handler<Mouse> HandleOnDeath;
	Handler<Mouse> HandleOnRevive;

private:
	int player = 0;
	float moveX;
	float moveY;
	float aimX;
	float aimY;
	bool interact;
	bool shoot;
	bool drop;
	Color initialColor;
	Renderable* render;
	Pickup* baseItem;
	Contraption* newItem;
	PhysicsComponent* _phys;
	std::set<PhysObjectType::PhysObjectType> checkFor;
	PhysicsComponent* _collidedObjects;
};

