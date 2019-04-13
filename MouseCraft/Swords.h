#pragma once
#include "Contraption.h"
#include "Physics/PhysObjectType.h"

class Mouse;

class Swords :
	public Contraption
{
public:
	Swords();
	~Swords();

	bool use(Mouse* m) override;
	void show() override;
	void OnCollision(PhysicsComponent* other);
	virtual void OnInitialized() override;
	virtual void Update(float dt) override;

private:	
	const float RADIUS = 5.0f;
	const int DAMAGE = 3;
	PhysicsComponent* _phys;
	PhysicsComponent* _collidedObjects;
	std::set<PhysObjectType::PhysObjectType> checkFor;
	Handler<Swords, PhysicsComponent*> HandleOnCollision;

	static Component* Create(json json);
	static PrefabRegistrar reg;
};

