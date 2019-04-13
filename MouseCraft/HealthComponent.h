#pragma once
#include <utility>
#include "Core/Component.h"
#include "Event/ISubscriber.h"
#include "Event/EventManager.h"
#include "Core/Entity.h"
#include "Event/Subject.h"

#include "Loading/PrefabLoader.h"
#include "json.hpp"
using json = nlohmann::json;

class HealthComponent : public Component
{
public:
	HealthComponent() {}

	virtual void OnInitialized() {}

	virtual ~HealthComponent() {}

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

	/* TEMPLATE
	{
		"type": "Health",
		"hp": 1,
		"shield": false
	}
	*/
	static Component* Create(json json);
	static PrefabRegistrar reg;
};