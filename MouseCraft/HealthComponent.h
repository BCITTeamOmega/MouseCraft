#pragma once
#include "Core/Component.h"
#include "Event/ISubscriber.h"
#include "Event/EventManager.h"

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
			auto p = static_cast<TypeParam<int>*>(params)->Param;
			if ((_health += p) <= 0)
			{
				//notify death
			}
		}
	}
};