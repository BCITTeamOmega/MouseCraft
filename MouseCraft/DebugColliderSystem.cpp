#include "DebugColliderSystem.h"

#include "DebugColliderComponent.h"
#include "Core/Entity.h"
#include <glm/glm.hpp>

DebugColliderSystem::DebugColliderSystem()
{
}


DebugColliderSystem::~DebugColliderSystem()
{
}

bool DebugColliderSystem::CheckCollision(DebugColliderComponent * left, DebugColliderComponent * right)
{
	auto range = left->radius + right->radius;
	auto dist = glm::distance(
		left->GetEntity()->transform.getWorldPosition(),
		right->GetEntity()->transform.getWorldPosition());
	return dist < range;
}

void DebugColliderSystem::Update(float dt)
{
	auto colliders = ComponentManager<DebugColliderComponent>::Instance().All();

	for (int i = 0; i < colliders.size(); ++i)
	{
		for (int j = i + 1; j < colliders.size(); ++j)
		{
			auto left = colliders[i];
			auto right = colliders[j];
			if (CheckCollision(left, right))
			{
				left->OnCollide.Notify(left, right);
				right->OnCollide.Notify(right, left);
			}
		}
	}
}
