#pragma once
#include "Contraption.h"
#include "Physics/PhysicsComponent.h"
#include "Event/Handler.h"
#include "Event/EventManager.h"

/*
Layout of the coil 

Entity (Mouse)
	Entity (contraption)
	- Coil
	- Physics
		Entity (field)
		- Renderable 
*/

class Mouse;

class Coil :
	public Contraption 
{
public:
	Coil();
	~Coil();

	bool use(Mouse* m) override;
	void show() override;
	void OnCollision(PhysicsComponent* other);
	virtual void OnInitialized() override;
	virtual void Update(float dt) override;

	// reference to field visuals
	Entity* fieldEntity;

private:
	// degree of speed when cat enters the field
	const float SLOW_MULTIPLIER = 0.25f;
	
	// range of the field
	const float FIELD_RANGE = 16.0F;

	// todo: damage

	// reference to physics component (NOT USED)
	PhysicsComponent* _phys;

	// if the field has been placed
	bool _isPlaced = false;

	// cat speed before being slowed 
	float _catSpeed;

	// the cat this field is currently colliding with
	PlayerComponent* _collidedCat;

	// the floor which to check the cat for
	std::set<PhysObjectType::PhysObjectType> checkFor;
	
	Handler<Coil, PhysicsComponent*> HandleOnCollision;

	static Component* Create(json json);
	static PrefabRegistrar reg;
};