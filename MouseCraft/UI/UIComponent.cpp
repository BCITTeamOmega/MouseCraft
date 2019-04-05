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
    parent = nullptr;
    aspectRatio = 1;

    ClickAction = "";

	z = 0;
	zForce = -1;

	color = { 0,0,0,0 };

    // Initialize Renderable data with a standard quad
	models = new std::vector<Model*>();
	ModelGen::makeQuad(ModelGen::Axis::Z, 1, 1);
}

UIComponent::~UIComponent() {
	for (Model* model : *models) {
		delete model;
	}
}

void UIComponent::Resize() {
    if (parent != nullptr) {
        // Calculate pixel size of panel based on Unit Type
        switch (xType) {
        case UNIT_PIXEL:
            screenSize.x = size.x;
            break;
        case UNIT_PERCENT:
            screenSize.x = parent->screenSize.x * (size.x / 100.0f);
            break;
        }
        switch(yType) {
        case UNIT_PIXEL:
            screenSize.y = size.y;
            break;
        case UNIT_PERCENT:
            screenSize.y = parent->screenSize.y * (size.y / 100.0f);
            break;
        case UNIT_SCALE:
            screenSize.y = screenSize.x / aspectRatio;
            break;
        }
        if (xType == UNIT_SCALE) {
            screenSize.x = screenSize.y * aspectRatio;
        }

		calculateScreenPosition();
    }
    
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

void UIComponent::setupModels() {
	(*models)[0] = ModelGen::makeQuad(ModelGen::Axis::Z, screenSize.x, screenSize.y);
	this->GetEntity()->transform.setLocalPosition(glm::vec3(
		screenPosition.x + screenSize.x / 2,
		screenPosition.y + screenSize.y / 2,
		z));
}

bool UIComponent::IsTransparent() const {
    return color.a < 1.0f;
}

void UIComponent::calculateScreenPosition() {
	// Calculate Anchor Position of panel based on anchor unit type
	glm::vec2 screenAnchor;
	screenAnchor.x = anchorXType == ANCHOR_PERCENT ? anchor.x / 100.0f * parent->screenSize.x : anchor.x;
	screenAnchor.y = anchorYType == ANCHOR_PERCENT ? anchor.y / 100.0f * parent->screenSize.y : anchor.y;

	// Calculate absolute position of panel based on parent panel and anchor types
	switch (hAnchor) {
	case ANCHOR_LEFT:
		screenPosition.x = parent->screenPosition.x + screenAnchor.x;
		break;
	case ANCHOR_HCENTER:
		screenPosition.x = parent->screenPosition.x + parent->screenSize.x / 2 - screenSize.x / 2 + screenAnchor.x;
		break;
	case ANCHOR_RIGHT:
		screenPosition.x = parent->screenPosition.x + parent->screenSize.x - screenSize.x - screenAnchor.x;
		break;
	}

	switch (vAnchor) {
	case ANCHOR_TOP:
		screenPosition.y = parent->screenPosition.y + parent->screenSize.y - screenSize.y - screenAnchor.y;
		break;
	case ANCHOR_VCENTER:
		screenPosition.y = parent->screenPosition.y + parent->screenSize.y / 2 - screenSize.y / 2 + screenAnchor.y;
		break;
	case ANCHOR_BOTTOM:
		screenPosition.y = parent->screenPosition.y + screenAnchor.y;
		break;
	}

	if (zForce >= 0) {
		z = zForce;
	}
	else {
		z = parent->z + 1;
	}
}
