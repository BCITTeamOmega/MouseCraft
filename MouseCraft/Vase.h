#pragma once

#include "Obstacle.h"

class Vase : public Obstacle
{
public:
	Vase();
	~Vase();

	virtual void OnInitialized() override;
	virtual void Update(float deltaTime) override;
	virtual void HitByCat(Vector2D dir) override;
	virtual void DestroyedByMouse() override;

	// reference to field visuals
	Entity* visualsEntity;

private:
	// deal one tick of damage every n seconds
	const float DAMAGE_RATE = 1.0f;

	// range of the field
	const float FIELD_RANGE = 16.0F;

	// if the field has been placed
	bool _isPlaced = false;

	// the floor which to check the cat for
	std::set<PhysObjectType::PhysObjectType> _checkFor;

	float _counter = 0;

	std::map<PhysicsComponent*, Vector2D*> _affected;
	std::vector<PhysicsComponent*> _found;

};