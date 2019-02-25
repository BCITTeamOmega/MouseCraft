#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif // !GLM_ENABLE_EXPERIMENTAL

#include <iomanip>
#include <glm/gtx/string_cast.hpp>
#include "Pickup.h"
#include "Contraption.h"
#include "ContraptionFactory.h"
#include "DebugColliderComponent.h"
#include "Core/OmegaEngine.h"
#include "Core/Entity.h"
#include "Core/Component.h"
#include "Core/UpdatableComponent.h"
#include "Event/EventManager.h"
#include "Event/Observer.h"
#include "Input/InputSystem.h"

class Mice : public UpdatableComponent, public Observer<DebugColliderComponent*, DebugColliderComponent*>
{
public:
	Mice();
	~Mice();

	virtual void OnInitialized() override;
	virtual void Update(float deltaTime);
	virtual void Notify(EventName eventName, Param *params) override;
	virtual void Publish(DebugColliderComponent* me, DebugColliderComponent* other) override;
	void addItem(Pickup* item);
	void dropItem();
	void use(Contraption* item);
	void combine(Pickup *material);

public:
	int player = 0;
	float speed = 10.0f;

private:
	float moveX;
	float moveY;
	float aimX;
	float aimY;
	bool shoot;
	bool drop;
	Pickup* baseItem;
	Contraption* newItem;
};

