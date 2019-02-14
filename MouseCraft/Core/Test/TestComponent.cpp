#include "TestComponent.h"

#include <iostream>
#include "../Entity.h"
#include "../OmegaEngine.h"
#include "../../Event/EventManager.h"

TestComponent::TestComponent(Entity* target) 
	: target(target)
{
}

TestComponent::~TestComponent()
{
}

void TestComponent::Update(float dt)
{
	std::cout << "TestComponent updated" << std::endl;
	++_frame;

	if (_frame == 1)
	{
		SDL_assert(GetEntity()->GetActive() && "Entity was not active when added to scene");
		SDL_assert(GetInitialized() && "Component was not initialized when added to scene");

		target->SetEnabled(false);
		SDL_assert(target->GetActive(), "Target entity action was not deferred");
	}
	else if (_frame == 2)
	{
		SDL_assert(!target->GetActive() && "Target entity deferred action was not executed");
		target->Destroy();
		target->GetActive();	// should not be Destroyed - if crash then failed. 
	}
	else if (_frame == 3)
	{
		auto id = target->GetID();	// this should crash or be jibberish ... heh terrible test!
		std::cout << "Should be gibberish - ID: " << id << std::endl;
		
		auto e = new Entity();
		OmegaEngine::Instance().AddEntity(e);
		SDL_assert(OmegaEngine::Instance().GetActiveScene()->root.GetChildren().size() == 1
			&& "Adding entity was not deferred");
	}
	else if (_frame == 4)
	{
		SDL_assert(OmegaEngine::Instance().GetActiveScene()->root.GetChildren().size() == 2
			&& "Adding entity was not executed");
		SetEnabled(false);
	}
	else if (_frame == 5)
	{
		SDL_assert(false && "Component disable failed!");
	}
}
