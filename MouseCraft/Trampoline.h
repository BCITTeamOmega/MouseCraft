#pragma once
#include "Contraption.h"
#include "Mouse.h"

class Mouse;

class Trampoline : public Contraption
{
public:
	Trampoline();
	~Trampoline();

	bool use(Mouse* m) override;
	void show() override;
	void OnCollision(PhysicsComponent* other);
	virtual void OnInitialized() override;
	virtual void Update(float dt) override;

	Entity* fieldEntity;

private: 
	bool _isPlaced = false;
	std::set<PhysObjectType::PhysObjectType> checkFor;
	Handler<Trampoline, PhysicsComponent*> HandleOnCollision;

	static Component* Create(json json);
	static PrefabRegistrar reg;
};

