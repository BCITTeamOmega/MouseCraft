#include "ExampleSystem.h"

#include <iostream>
#include "../ComponentManager.h"
#include "../Entity.h"
#include "ExampleComponent.h"
#include "glm/gtx/string_cast.hpp"

ExampleSystem::ExampleSystem()
{
}


ExampleSystem::~ExampleSystem()
{
}

void ExampleSystem::Update(float dt)
{
	auto components = ComponentManager<ExampleComponent>::Instance().All();

	for (auto& c : components)
	{
		if (c->GetActive())
		{
			auto e = c->GetEntity();
			e->transform.setLocalPosition(
				e->transform.getLocalPosition() + c->velocity * dt);
			
			std::cout << "ExampleEntity: " << e->GetID() 
				<< " position: " << glm::to_string(e->transform.getLocalPosition()) << std::endl;
		}
	}
}
