#include "UIManager.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include "../Core/ComponentManager.h"
#include <stack>

using std::vector;

UIManager::UIManager(){}

void UIManager::Update(float dt) {
	vector<UIComponent*> rootComponents;
	
	auto uiComponents = ComponentManager<UIComponent>::Instance().All();
	for (auto component : uiComponents) {
		Entity* e = component->GetEntity();
		if (e != nullptr) {
			Entity* parent = e->GetParent();
			if (parent == nullptr || parent->GetComponent<UIComponent>() == nullptr) {
				// If the entity doesn't have a parent or has a parent that doesn't have a UIComponent
				// Then it is a root entity
				rootComponents.push_back(component);
			}
		}
	}

	for (UIComponent* component : rootComponents) {
		Resize(component);
	}
}

void UIManager::Resize(UIComponent* root) {
    root->Resize();
}

void UIManager::ClickUI(UIComponent* root, float x, float y) {
	UIComponent* select = root;
	findTopClick(&select, root, x, root->screenSize.y - y);
	if (select != root) {
		_clickFunctions[select->ClickAction]();
	}
}

void UIManager::DefineClickFunction(const std::string name, void(*f)()) {
	_clickFunctions[name] = f;
}

void UIManager::defineClicks() {

}

void UIManager::findTopClick(UIComponent** top, UIComponent* comp, const float x, const float y) {
	for (Entity* e : comp->GetEntity()->GetChildren()) {
		UIComponent* c = e->GetComponent<UIComponent>();
		if (c->visible && pointInRect(x, y, c->screenPosition.y + c->screenSize.y,
			c->screenPosition.x + c->screenSize.x, c->screenPosition.x, c->screenPosition.y)) {
			if (c->ClickAction != "" && c->z > (*top)->z) {
				*top = c;
			}
			findTopClick(top, c, x, y);
		}
	}
}

bool UIManager::pointInRect(float px, float py, float rTop, float rRight, float rLeft, float rBottom) {
    return (px > rLeft && px < rRight && py < rTop && px > rBottom);
}

void UIManager::traverseChildren(UIComponent *component) {
	if (component != nullptr) {
		// Resize if invalidated
		if (!component->valid) {
			component->Resize();
		}

		// Call recursively on children in DFS style
		for (Entity* e : component->GetEntity()->GetChildren()) {
			UIComponent* child = e->GetComponent<UIComponent>();
			if (child->visible) {
				traverseChildren(child);
			}
		}
	}
}