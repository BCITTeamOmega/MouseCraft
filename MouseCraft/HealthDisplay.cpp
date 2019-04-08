#include "HealthDisplay.h"
#include "Event/EventManager.h"
#include "Event/EventName.h"

HealthDisplay::HealthDisplay(int maxHealth) : _maxHealth(maxHealth) {
	EventManager::Subscribe(EventName::HEALTH_CHANGE, this);
}

HealthDisplay::~HealthDisplay() {
	EventManager::Unsubscribe(EventName::HEALTH_CHANGE, this);
}

void HealthDisplay::OnInitialized() {

}

void HealthDisplay::Update(float dt) {

}

void HealthDisplay::Notify(EventName eventName, Param* params) {
	if (eventName == EventName::HEALTH_CHANGE) {
		auto data = static_cast<TypeParam<>*>(params)->Param;
	}
}

void HealthDisplay::setHealthUI(UIComponent* component) {
	_healthUI = component;
	_maxWidth = component->size.x;
}

void HealthDisplay::updateHealthUI(int health) {
	_healthUI->size.x = _maxWidth * health / _maxHealth;
}