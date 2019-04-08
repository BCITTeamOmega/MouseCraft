#include "HealthDisplay.h"

HealthDisplay::HealthDisplay(int maxHealth) : 
	_maxHealth(maxHealth), onHealthChange(this, &HealthDisplay::updateHealthUI)
{
}

HealthDisplay::~HealthDisplay() {
}

void HealthDisplay::OnInitialized() {

}

void HealthDisplay::Update(float dt) {

}

void HealthDisplay::setHealthUI(UIComponent* component) {
	_healthUI = component;
	_maxWidth = component->size.x;
}

void HealthDisplay::setWatchingHealthComponent(HealthComponent* component) {
	_healthWatch = component;
	onHealthChange.Observe(_healthWatch->OnHealthChanged);
}

void HealthDisplay::updateHealthUI(int health) {
	_healthUI->size.x = _maxWidth * health / _maxHealth;
}