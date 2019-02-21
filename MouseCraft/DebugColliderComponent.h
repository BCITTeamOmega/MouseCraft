#pragma once

#include "Core/Component.h"
#include "Subject.h"
#include "Observer.h"
#include <string>

class DebugColliderComponent : public Component,
	public Observer<DebugColliderComponent*, DebugColliderComponent*>
{
// functions 
public:
	DebugColliderComponent();
	~DebugColliderComponent();
	void Publish(DebugColliderComponent*, DebugColliderComponent*) override;

// variables 
public: 
	std::string tag; 
	float radius = 0.5f;
	Subject<DebugColliderComponent*, DebugColliderComponent*> OnCollide;
};

