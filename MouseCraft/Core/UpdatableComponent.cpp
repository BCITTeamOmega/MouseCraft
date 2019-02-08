#include "UpdatableComponent.h"

#include "../EventManager/EventManager.h"
#include "Entity.h"

UpdatableComponent::UpdatableComponent()
{
	EventManager::Subscribe(COMPONENT_UPDATE, this);
}

UpdatableComponent::~UpdatableComponent()
{
	EventManager::Unsubscribe(COMPONENT_UPDATE, this);
}

void UpdatableComponent::Notify(EventName eventName, Param * params)
{
	if (GetEnabled() && GetEntity() && GetEntity()->GetActive())
	{
		auto delta = static_cast<TypeParam<float>*>(params);
		Update(delta->Param);
	}
}
