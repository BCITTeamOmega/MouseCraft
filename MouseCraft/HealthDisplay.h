#pragma once
#include "Core/UpdatableComponent.h"
#include "UI/UIComponent.h"
class HealthDisplay : public UpdatableComponent {
	HealthDisplay(int maxHealth);
	~HealthDisplay();
	void OnInitialized() override;
	void Update(float dt) override;
	void Notify(EventName eventName, Param* params) override;
	void setHealthUI(UIComponent* component);
private:
	void updateHealthUI(int health);
	UIComponent* _healthUI;
	float _maxWidth;
	int _maxHealth;
};