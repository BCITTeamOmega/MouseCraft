#include "PlayerComponent.h"

unsigned int PlayerComponent::_curID = 0;

void PlayerComponent::OnInitialized()
{
	_health = GetEntity()->GetComponent<HealthComponent>();
	_playerID = _curID++;
}

void PlayerComponent::Notify(EventName eventName, Param* params)
{
	//do movement stuff
}