#include "MenuController.h"
#include "Event/EventManager.h"
#include "Input/InputSystem.h"
#include "UI/UIComponent.h"

#include <iostream>

MenuController::MenuController() {
	EventManager::Subscribe(EventName::INPUT_BUTTON, this);
	EventManager::Subscribe(EventName::INPUT_AXIS_2D, this);
}

MenuController::~MenuController() {
	EventManager::Unsubscribe(EventName::INPUT_BUTTON, this);
	EventManager::Unsubscribe(EventName::INPUT_AXIS_2D, this);
}

void MenuController::OnInitialized() {

}

void MenuController::Update(float dt) {

}

void MenuController::Notify(EventName eventName, Param* params) {
	UpdatableComponent::Notify(eventName, params);
	if (eventName == EventName::INPUT_BUTTON) {
		auto data = static_cast<TypeParam<ButtonEvent>*>(params)->Param;
		if (data.isDown != true) {
			return;
		}

		if (data.button == Button::PRIMARY) {
			select();
		}
	}
	else if (eventName == EventName::INPUT_AXIS_2D)
	{
		auto data = static_cast<TypeParam<Axis2DEvent>*>(params)->Param;
		if (data.axis == Axis::LEFT) {
			glm::vec2 axisAmnt = data.GetClamped();
			if (axisAmnt.y > 0.2 && _lastAxis.y <= 0.2) {
				down();
			}
			else if (axisAmnt.y < -0.2 && _lastAxis.y >= -0.2) {
				up();
			}
			_lastAxis = axisAmnt;
		}
	}
}

void MenuController::addMenuItem(Entity* e) {
	this->GetEntity()->AddChild(e);
	_menuItems.push_back(e);
	UIComponent* cmp = e->GetComponent<UIComponent>();
	if (_menuItems.size() == 1) {
		// Need to highlight the first element when it's added
		highlight();
	}
}

void MenuController::up() {
	unhighlight();
	currentSelect--;
	if (currentSelect < 0) {
		currentSelect = _menuItems.size() - 1;
	}
	std::cout << "Currently selected: " << currentSelect << std::endl;
	highlight();
}

void MenuController::down() {
	unhighlight();
	currentSelect++;
	if (currentSelect >= _menuItems.size()) {
		currentSelect = 0;
	}
	std::cout << "Currently selected: " << currentSelect << std::endl;
	highlight();
}

void MenuController::unhighlight() {
	UIComponent* uiRend = _menuItems[currentSelect]->GetComponent<UIComponent>();
	if (uiRend != nullptr) {
		uiRend->color = Color(1.0f, 1.0f, 1.0f);
	}
}

void MenuController::highlight() {
	UIComponent* uiRend = _menuItems[currentSelect]->GetComponent<UIComponent>();
	if (uiRend != nullptr) {
		uiRend->color = Color(1.0f, 0.2f, 0.2f);
	}
}