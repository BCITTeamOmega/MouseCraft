#pragma once
#include "Core/UpdatableComponent.h"
#include "Core/Entity.h"
#include "glm/glm.hpp"
#include <functional>
class MenuController : public UpdatableComponent {
public:
	MenuController();
	~MenuController();
	void OnInitialized() override;
	void Update(float dt) override;
	void Notify(EventName eventName, Param* params) override;
	void onSelect(std::function<void(int)> c) {
		_selectCallback = c;
	}
	void select() {
		if (_selectCallback != nullptr) {
			_selectCallback(currentSelect);
		}
	}
	void addMenuItem(Entity* e);
	void up();
	void down();
private:
	void unhighlight();
	void highlight();
	glm::vec2 _lastAxis;
	std::function<void(int)> _selectCallback;
	std::vector<Entity*> _menuItems;
	int currentSelect;
};