#pragma once
#include "Core/UpdatableComponent.h"
#include "UI/UIComponent.h"
#include "HealthComponent.h"
#include "Event/Handler.h"

class HealthDisplay : public UpdatableComponent {
public:
	HealthDisplay(int maxHealth);
	~HealthDisplay();
	void OnInitialized() override;
	void Update(float dt) override;
	void setHealthUI(UIComponent* component);
	void setWatchingHealthComponent(HealthComponent* component);
private:
	Handler<HealthDisplay, int> onHealthChange;
	void updateHealthUI(int health);
	UIComponent* _healthUI;
	HealthComponent* _healthWatch;
	float _maxWidth;
	int _maxHealth;
};