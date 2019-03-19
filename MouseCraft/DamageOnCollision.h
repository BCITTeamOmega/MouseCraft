#pragma once

#include "Core/Component.h"
#include "Event/Handler.h"
#include "Physics/PhysicsComponent.h"

class DamageOnCollision : public Component
{
public:
	DamageOnCollision();
	DamageOnCollision(std::set<PhysObjectType::PhysObjectType> checkFor);
	~DamageOnCollision();
	virtual void OnInitialized() override;
	void SetLayers(std::set<PhysObjectType::PhysObjectType> checkFor);

	int damage = 1;
	bool destroyOnCollision = true;

private: 
	void OnCollision(PhysicsComponent* other);
	Handler<DamageOnCollision, PhysicsComponent*> _handleOnCollision;
	std::set<PhysObjectType::PhysObjectType> _checkFor;
};

