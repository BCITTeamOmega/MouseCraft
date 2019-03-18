#pragma once
#include <utility>
#include "Core/Component.h"
#include "Event/ISubscriber.h"
#include "Event/EventManager.h"
#include "Core/Entity.h"
#include "Event/Subject.h"

#include "json.hpp"
using json = nlohmann::json;

class HealthComponent : public Component, public ISubscriber
{
public:
	HealthComponent()
	{

	}

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
	Subject<> OnRevive;


	void Damage(int dmg)
	{
		if (dmg == 0)
			return;

		if (_shield)
		{
			RemoveShield();
			return;
		}

		if ((_health -= dmg) <= 0)
		{
			_health = 0;
			OnDeath.Notify();
		}
		OnHealthChanged.Notify(_health);
	}

	void SetHealth(int health)
	{
		auto old = _health;
		_health = health;
		
		if (health <= 0)
		{
			// set health to dead 
			_health = 0;
			OnDeath.Notify();
		}
		else if (old == 0)
		{
			// set health to alive (and was dead)
			OnRevive.Notify();
		}

		OnHealthChanged.Notify(_health);
	}

	void AddShield()
	{
		_shield = true;
	}

	void RemoveShield()
	{
		_shield = false;
	}

private:
	int _health = 1;
	bool _shield = false;

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