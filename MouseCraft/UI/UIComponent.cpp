#include "UIComponent.h"
#include "../Graphics/ModelGen.h"

UIComponent::UIComponent(float width, float height, float x, float y) :
    size(width, height), anchor(x, y) {

    // Set default values for a panel
	id = "";
    visible = true;
	valid = true;
    vAnchor = ANCHOR_TOP;
    hAnchor = ANCHOR_LEFT;
    anchorXType = ANCHOR_PIXEL;
    anchorYType = ANCHOR_PIXEL;
    xType = UNIT_PERCENT;
    yType = UNIT_PERCENT;
    aspectRatio = 1;

    ClickAction = "";
	zForce = -1;
	z = 0;

	color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    // Initialize Renderable data with a standard quad
	models = std::vector<Model*>();
}

UIComponent::~UIComponent() {
	for (Model* model : models) {
		delete model;
	}
}

void UIComponent::Resize() {
	if (this->GetEntity() != nullptr) {
		glm::vec2 parentSize = { 1.0f, 1.0f };
		Entity* parent = this->GetEntity()->GetParent();
		if (parent != nullptr && parent->GetComponent<UIComponent>() != nullptr) {
			UIComponent* cmp = parent->GetComponent<UIComponent>();
			parentSize = cmp->screenSize;
		}

		// Calculate pixel size of panel based on Unit Type
		switch (xType) {
		case UNIT_PIXEL:
			screenSize.x = size.x;
			break;
		case UNIT_PERCENT:
			screenSize.x = parentSize.x * (size.x / 100.0f);
			break;
		}
		switch (yType) {
		case UNIT_PIXEL:
			screenSize.y = size.y;
			break;
		case UNIT_PERCENT:
			screenSize.y = parentSize.y * (size.y / 100.0f);
			break;
		case UNIT_SCALE:
			screenSize.y = screenSize.x / aspectRatio;
			break;
		}
		if (xType == UNIT_SCALE) {
			screenSize.x = screenSize.y * aspectRatio;
		}

		calculateScreenPosition();

		// Generate vertices of quad from position and size of panel
		setupModels();

		// Iterate resize on child panels
		auto children = this->GetEntity()->GetChildren();
		for (Entity *child : children) {
			UIComponent* comp = child->GetComponent<UIComponent>();
			if (comp != nullptr) {
				comp->Resize();
			}
		}

		valid = true;
	}
}

void UIComponent::setupModels() {
	models.clear();
	models.push_back(ModelGen::makeQuad(ModelGen::Axis::Z, screenSize.x, screenSize.y));

	// Shenanigans to set the position independent of parent location
	// This is fine since the parents are resized first
	glm::vec3 parentPos = { 0.0f, 0.0f, 0.0f };
	Entity* parent = this->GetEntity()->GetParent();
	if (parent != nullptr) {
		parentPos = parent->transform.getWorldPosition();
	}
	this->GetEntity()->transform.setLocalPosition(glm::vec3(
		screenPosition.x + screenSize.x / 2 - parentPos.x,
		screenPosition.y + screenSize.y / 2 - parentPos.y,
		z - parentPos.z));
}

bool UIComponent::IsTransparent() {
    return color.getAlpha() < 1.0f;
}

void UIComponent::calculateScreenPosition() {
	Entity* e = GetEntity();
	if (e != nullptr) {
		glm::vec2 parentSize = { 1.0f, 1.0f };
		glm::vec2 parentPosition = { 0.0f, 0.0f };
		float parentZ = 0.0f;
		Entity* parent = this->GetEntity()->GetParent();
		if (parent != nullptr && parent->GetComponent<UIComponent>() != nullptr) {
			UIComponent* cmp = parent->GetComponent<UIComponent>();
			parentSize = cmp->screenSize;
			parentPosition = cmp->screenPosition;
			parentZ = cmp->z;
		}
		// Calculate Anchor Position of panel based on anchor unit type
		glm::vec2 screenAnchor;
		screenAnchor.x = anchorXType == ANCHOR_PERCENT ? anchor.x / 100.0f * parentSize.x : anchor.x;
		screenAnchor.y = anchorYType == ANCHOR_PERCENT ? anchor.y / 100.0f * parentSize.y : anchor.y;

		// Calculate absolute position of panel based on parent panel and anchor types
		switch (hAnchor) {
		case ANCHOR_LEFT:
			screenPosition.x = parentPosition.x + screenAnchor.x;
			break;
		case ANCHOR_HCENTER:
			screenPosition.x = parentPosition.x + parentSize.x / 2 - screenSize.x / 2 + screenAnchor.x;
			break;
		case ANCHOR_RIGHT:
			screenPosition.x = parentPosition.x + parentSize.x - screenSize.x - screenAnchor.x;
			break;
		}

		switch (vAnchor) {
		case ANCHOR_TOP:
			screenPosition.y = parentPosition.y + parentSize.y - screenSize.y - screenAnchor.y;
			break;
		case ANCHOR_VCENTER:
			screenPosition.y = parentPosition.y + parentSize.y / 2 - screenSize.y / 2 + screenAnchor.y;
			break;
		case ANCHOR_BOTTOM:
			screenPosition.y = parentPosition.y + screenAnchor.y;
			break;
		}

		if (zForce >= 0) {
			z = zForce;
		}
		else {
			z = parentZ + 1;
		}
	}
}
