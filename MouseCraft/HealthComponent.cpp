#include "HealthComponent.h"

Component* HealthComponent::Create(json json)
{
	auto c_health = ComponentManager<HealthComponent>::Instance().Create<HealthComponent>();
	c_health->_health = json["hp"].get<int>();
	c_health->_shield = json["shield"].get<bool>();
	return c_health;
}

PrefabRegistrar HealthComponent::reg("Health", &HealthComponent::Create);