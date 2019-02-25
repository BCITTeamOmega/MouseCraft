#pragma once
#include <utility>
#include "Core/Component.h"
#include "Event/ISubscriber.h"
#include "Event/EventManager.h"
#include "Core/Entity.h"

class HealthComponent : public Component, public ISubscriber
{
public:
	virtual void OnInitialized()
	{
		EventManager::Subscribe(HEALTH_CHANGE, this);
	}

	virtual ~HealthComponent()
	{
		EventManager::Unsubscribe(HEALTH_CHANGE, this);
	}

	int GetHealth() { return _health; }

private:
	int _health = 1;

	void Notify(EventName eventName, Param* params)
	{
		if (eventName == HEALTH_CHANGE)
		{
			auto p = static_cast<TypeParam<std::pair<int, int>>*>(params)->Param;
			if (p.first != GetEntity()->GetID())
				return;
			if ((_health += p.second) <= 0)
			{
				//notify death
			}
		}
	}
};