#pragma once
#include "Contraption.h"
class Platform :
	public Contraption
{
public:
	Platform();
	~Platform();

	bool use() override;
	void show() override;
	void OnCollision(PhysicsComponent* other);
	virtual void OnInitialized() override;
	virtual void Update(float dt) override;

	Entity* fieldEntity;

private: 
	bool _isPlaced = false;
	PlayerComponent* _collidedMice;
	std::set<PhysObjectType::PhysObjectType> checkFor;
	Handler<Platform, PhysicsComponent*> HandleOnCollision;
};

