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

void UpdatableComponent::onInitialized()
{
	// perferably sub would happen here (don't want to listen for updates right away.)
	// WARNING: If a component makes a component in their update() func, may cause error...
}

void UpdatableComponent::Notify(EventName eventName, Param * params)
{
	auto f = getEntity();
	if (getEnabled() && getEntity() && getEntity()->getActive())
	{
		auto delta = static_cast<TypeParam<float>*>(params);
		update(delta->Param);
	}
}
