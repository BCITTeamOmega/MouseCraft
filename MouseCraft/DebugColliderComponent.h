#pragma once

#include "Core/Component.h"

class DebugColliderComponet : public Component
{
// functions 
public:
	DebugColliderComponet();
	~DebugColliderComponet();

// variables 
public: 
	float radius = 0.5f;
};

