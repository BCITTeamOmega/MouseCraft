#include "PlayerComponent.h"

void PlayerComponent::OnInitialized()
{
	_health = GetEntity()->GetComponent<HealthComponent>();
}

void PlayerComponent::Notify(EventName eventName, Param* params)
{
	
}