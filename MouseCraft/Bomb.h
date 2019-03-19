#pragma once
#include "Contraption.h"
#include "DamageOnCollision.h"
#include "TimedDestruction.h"
#include "Physics/PhysicsComponent.h"

class Bomb :
	public Contraption
{
public:
	const static float RADIUS;   // = 10.0f;
	const static float LIFETIME; // = 2.0f;
	const static float SPEED;    // = 12.5f;
	const static int DAMAGE;     // = 2;

	Bomb();
	~Bomb();

	virtual void OnInitialized() override;

	bool use() override;
	
	void show() override;

	// called when this component is destroyed 
	void Explode();

private: 
	PhysicsComponent* _physics;
	TimedDestruction* _timed;
	DamageOnCollision* _dcollision;
	std::set<PhysObjectType::PhysObjectType> _checkFor;
};

