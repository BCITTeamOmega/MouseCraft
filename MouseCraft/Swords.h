#pragma once
#include "Contraption.h"
#include "Physics/PhysObjectType.h"

class Swords :
	public Contraption
{
public:
	Swords();
	~Swords();

	bool use() override;
	void show() override;
	void OnCollision(PhysicsComponent* other);
	virtual void OnInitialized() override;
	virtual void Update(float dt) override;

private:	
	PhysicsComponent* _phys;
	PhysicsComponent* _collidedObjects;
	std::set<PhysObjectType::PhysObjectType> checkFor;
	Handler<Swords, PhysicsComponent*> HandleOnCollision;
};

