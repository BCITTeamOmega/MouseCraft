#include "TextComponent.h"
#include "../Graphics/ModelGen.h"

const std::string TextComponent::DEFAULT_FONT = "res/fonts/ShareTechMono.png";

TextComponent::TextComponent(std::string text, float fontSize, float x, float y, std::string fontPath) :
    UIComponent(0, 0, x, y), _text(text), _fontSize(fontSize), _spacing(1.0f), _fontPath(fontPath) {
}

void TextComponent::SetText(std::string text) {
    if (text != _text) { // If text is different, invalidate this UIComponent to be Resized
        _text = text;
        valid = false;
    }
}

void TextComponent::Resize() {
	if (this->GetEntity() != nullptr) {
		glm::vec2 parentSize = { 1.0f, 1.0f };
		Entity* parent = this->GetEntity()->GetParent();
		if (parent != nullptr && parent->GetComponent<UIComponent>() != nullptr) {
			UIComponent* cmp = parent->GetComponent<UIComponent>();
			parentSize = cmp->screenSize;
		}
		screenSize.y = yType == UNIT_PERCENT ? parentSize.y * _fontSize / 100.0f : _fontSize;

		float fontWidth = screenSize.y * _spacing;
		screenSize.x = fontWidth * _text.length();

		calculateScreenPosition();

		generateVertices();

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

bool TextComponent::IsTransparent() { return true; }

glm::vec2 TextComponent::getUVfromChar(const char c) {
    int ascii = int(c);

    int index = 0;
    if (ascii >= 32 && ascii <= 126) {
        index = ascii - 32;
    } else if (ascii == 8216 || ascii == 8217) {
        index = ascii - 8121;
    } else if (ascii == 8220 || ascii == 8221) {
        index = ascii - 8123;
    }

    glm::vec2 uv;
    uv.x = 0.1f * (index % 10);
    uv.y = 1.0f - (0.1f * (index / 10));

    return uv;
}

void TextComponent::generateVertices() {
    glm::vec2 uv;
    //float fontWidth = screenSize.y * texture.width / texture.height;
	float fontWidth = screenSize.y * _spacing;

	models.clear();

    for (int i = 0; i < _text.length(); i++) {
		/*
		positions.push_back(glm::vec3(screenPosition.x + fontWidth * i, screenPosition.y + screenSize.y, 0)); // Top Left
		positions.push_back(glm::vec3(screenPosition.x + fontWidth * i + fontWidth, screenPosition.y + screenSize.y, 0)); // Top Right
		positions.push_back(glm::vec3(screenPosition.x + fontWidth * i, screenPosition.y, 0)); // Bottom Left
		positions.push_back(glm::vec3(screenPosition.x + fontWidth * i + fontWidth, screenPosition.y, 0)); // Bottom Right
		
		UVs.push_back(uv);
		UVs.push_back(uv + glm::vec2(0.1, 0));
		UVs.push_back(uv + glm::vec2(0, -0.1));
		UVs.push_back(uv + glm::vec2(0.1, -0.1));

		elements.push_back(i * 4 + 1);
		elements.push_back(i * 4 + 0);
		elements.push_back(i * 4 + 2);
		elements.push_back(i * 4 + 1);
		elements.push_back(i * 4 + 2);
		elements.push_back(i * 4 + 3);
		*/
		Model* m = ModelGen::makeQuad(ModelGen::Axis::Z, fontWidth, screenSize.y);
		auto& vec = m->getGeometry()->getVertexData();
		for (int j = 0; j < vec.size(); j += 3) {
			vec[j] += fontWidth * i;
		}

		uv = getUVfromChar(_text[i]);
		auto& uvs = m->getGeometry()->getTexCoordData();
		uvs[0] = uv.x;
		uvs[1] = uv.y - 0.1;
		uvs[2] = uv.x + 0.1;
		uvs[3] = uv.y - 0.1;
		uvs[4] = uv.x;
		uvs[5] = uv.y;
		uvs[6] = uv.x + 0.1;
		uvs[7] = uv.y;

		m->setTexture(new std::string(_fontPath));

		models.push_back(m);
    }

	// Shenanigans to set the position independent of parent location
	// This is fine since the parents are resized first
	glm::vec3 parentPos = { 0.0f, 0.0f, 0.0f };
	Entity* parent = this->GetEntity()->GetParent();
	if (parent != nullptr) {
		parentPos = parent->transform.getWorldPosition();
	}
	this->GetEntity()->transform.setLocalPosition(glm::vec3(
		screenPosition.x + fontWidth / 2 - parentPos.x,
		screenPosition.y + screenSize.y / 2 - parentPos.y,
		z - parentPos.z));
}

void TextComponent::SetSpacing(float spacing) {
	_spacing = spacing;
}

float TextComponent::GetSpacing() {
	return _spacing;
}