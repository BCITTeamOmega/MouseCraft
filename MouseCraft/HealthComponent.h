#pragma once
#include <utility>
#include "Core/Component.h"
#include "Event/ISubscriber.h"
#include "Event/EventManager.h"
#include "Core/Entity.h"
#include "Event/Subject.h"

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

	Subject<int> OnHealthChanged;
	Subject<> OnDeath;


	void CutSelf(int dmg)
	{
		if (dmg == 0)
			return;
		if ((_health -= dmg) < 0)
		{
			_health = 0;
			OnDeath.Notify();
		}
		OnHealthChanged.Notify(_health);
	}

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