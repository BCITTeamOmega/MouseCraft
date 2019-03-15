#include "ContraptionSystem.h"

#include "Core/Component.h"
#include "Core/ComponentManager.h"
#include "Contraption.h"

ContraptionSystem::ContraptionSystem()
{
}


ContraptionSystem::~ContraptionSystem()
{
}

void ContraptionSystem::Update(float deltaSeconds)
{
	auto components = ComponentManager<Contraption>::Instance().All();
	for (auto& c : components)
	{
		if (!c->GetActive()) continue;
		c->Update(deltaSeconds);
	}
}
