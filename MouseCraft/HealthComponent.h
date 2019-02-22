#pragma once
#include "Core/Component.h"
#include "Event/ISubscriber.h"

class HealthComponent : public Component, public ISubscriber
{
public:
	virtual void OnInitialized() {}
private:
	unsigned int _health = 1;

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