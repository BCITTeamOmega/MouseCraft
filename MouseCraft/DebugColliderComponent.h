#pragma once

#include "Core/Component.h"
#include "Subject.h"
#include "Observer.h"

class DebugColliderComponent : public Component,
	public Observer<DebugColliderComponent*, DebugColliderComponent*>
{
// functions 
public:
	DebugColliderComponent();
	~DebugColliderComponent();
	void Update(DebugColliderComponent*, DebugColliderComponent*) override;

// variables 
public: 
	float radius = 0.5f;
	Subject<DebugColliderComponent*, DebugColliderComponent*> OnCollide;
};

