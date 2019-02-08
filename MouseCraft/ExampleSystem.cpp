#include "ExampleSystem.h"

#include <iostream>
#include "Core/ComponentManager.h"
#include "Core/Entity.h"
#include "ExampleComponent.h"
#include "glm/gtx/string_cast.hpp"

ExampleSystem::ExampleSystem()
{
}


ExampleSystem::~ExampleSystem()
{
}

void ExampleSystem::update(float dt)
{
	auto components = ComponentManager<ExampleComponent>::Instance().All();

	for (auto& c : components)
	{
		if (c->getActive())
		{
			auto e = c->getEntity();
			e->transform.setLocalPosition(
				e->transform.getLocalPosition() + c->velocity * dt);
			
			std::cout << "ExampleEntity: " << e->getID() 
				<< " position: " << glm::to_string(e->transform.getLocalPosition()) << std::endl;
		}
	}
}
