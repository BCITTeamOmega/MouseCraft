#pragma once
#include "Contraption.h"
#include "DamageOnCollision.h"
#include "TimedDestruction.h"
#include "Physics/PhysicsComponent.h"
#include "Rotator.h"

class Mouse;

class Bomb :
	public Contraption
{
public:
	const static float RADIUS;   // = 10.0f;
	const static float LIFETIME; // = 2.0f;
	const static float SPEED;    // = 12.5f;
	const static int DAMAGE;     // = 2;
	const static float EXPLOSION_LIFETIME;	// = 0.6f;
	const static float VER_VEL;

	Bomb();
	~Bomb();

	virtual void OnInitialized() override;

	bool use(Mouse* m) override;
	
	void show() override;

	// called when this component is destroyed 
	void Explode();

	Entity* explosion;

private: 
	PhysicsComponent* _physics;
	TimedDestruction* _timed;
	DamageOnCollision* _dcollision;
	Rotator* _rotator;
	std::set<PhysObjectType::PhysObjectType> _checkFor;

	static Component* Create(json json);
	static PrefabRegistrar reg;
};

