#include "DebugColliderSystem.h"

#include "DebugColliderComponent.h"


DebugColliderSystem::DebugColliderSystem()
{
}


DebugColliderSystem::~DebugColliderSystem()
{
}

void DebugColliderSystem::Update(float dt)
{
	auto colliders = ComponentManager<DebugColliderComponet>::Instance().All();


}
