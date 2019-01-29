#include "TestComponent.h"

#include <iostream>
#include "../Entity.h"
#include "../OmegaEngine.h"
#include "../../EventManager/EventManager.h"

TestComponent::TestComponent(Entity* target) 
	: target(target)
{
}

TestComponent::~TestComponent()
{
}

void TestComponent::update(float dt)
{
	std::cout << "TestComponent updated" << std::endl;
	++_frame;

	if (_frame == 1)
	{
		SDL_assert(getEntity()->getActive() && "Entity was not active when added to scene");
		SDL_assert(getInitialized() && "Component was not initialized when added to scene");

		target->setEnabled(false);
		SDL_assert(target->getActive(), "Target entity action was not deferred");
	}
	else if (_frame == 2)
	{
		SDL_assert(!target->getActive() && "Target entity deferred action was not executed");
		target->destroy();
		target->getActive();	// should not be destroyed - if crash then failed. 
	}
	else if (_frame == 3)
	{
		auto id = target->getID();	// this should crash or be jibberish ... heh terrible test!
		std::cout << "Should be gibberish - ID: " << id << std::endl;
		
		auto e = new Entity();
		OmegaEngine::instance().addEntity(e);
		SDL_assert(OmegaEngine::instance().getActiveScene()->root.getChildren().size() == 1
			&& "Adding entity was not deferred");
	}
	else if (_frame == 4)
	{
		SDL_assert(OmegaEngine::instance().getActiveScene()->root.getChildren().size() == 2
			&& "Adding entity was not executed");
		setEnabled(false);
	}
	else if (_frame == 5)
	{
		SDL_assert(false && "Component disable failed!");
	}
}
