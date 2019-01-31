#include "TextComponent.h"

const std::string TextComponent::DEFAULT_FONT = "ShareTechMono.png";

TextComponent::TextComponent(std::string text, float fontSize, float x, float y, std::string fontType) :
    UIComponent(0, 0, x, y), _text(text), _fontSize(fontSize) {
	//TODO correct texture loading once renderer built
    //texture = AssetLoader::loadTexture(fontType);
}

void TextComponent::SetText(std::string text) {
    if (text != _text) { // If text is different, invalidate this UIComponent to be Resized
        _text = text;
        valid = false;
    }
}

void TextComponent::Resize() {
    if (parent != nullptr) {
        screenSize.y = yType == UNIT_PERCENT ? parent->screenSize.y * _fontSize / 100.0f : _fontSize;
        
		// TODO update once texture loading is complete
        //float fontWidth = screenSize.y * texture.width / texture.height;
        //screenSize.x = fontWidth * _text.length();

		calculateScreenPosition();
        
		generateVertices();

		repopulateBuffers();
    }

	valid = true;
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

	//TODO Update once texture loading complete
	float fontWidth = 0;
    //float fontWidth = screenSize.y * texture.width / texture.height;

	positions.clear();
	UVs.clear();
	elements.clear();

    for (int i = 0; i < _text.length(); i++) {
        uv = getUVfromChar(_text[i]);
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
    }
}