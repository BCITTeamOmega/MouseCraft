#pragma once

#include "Obstacle.h"

#include "Loading/PrefabLoader.h"
#include "json.hpp"
using json = nlohmann::json;

class Obstruction : public Obstacle
{
public:
	Obstruction();
	~Obstruction();
	void OnInitialized() override;
	void Update(float deltaTime) override;
	virtual void HitByCat(Vector2D dir) override;
	virtual void DestroyedByMouse() override;

private:
	void OnMouseCollide(PhysicsComponent* other);

private: 
	const float HIT_TRAVEL_DIST = 5.0f;
	bool _isFalling = false;
	PhysicsComponent* _physics;
	Handler<Obstruction, PhysicsComponent*> HandleMouseCollide;

	/* TEMPLATE
	{
		"type": "Obstruction",
	}
	*/
	static Component* Create(json json);
	static PrefabRegistrar reg;
};

