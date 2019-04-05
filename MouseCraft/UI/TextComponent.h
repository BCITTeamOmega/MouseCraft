#pragma once

#include "UIManager.h"

/**
Type of UIComponent that renders text to the panel, through the means of a bitmap font
*/
class TextComponent : public UIComponent {
public:
	// default font type to use
    static const std::string DEFAULT_FONT;

    TextComponent(std::string text, float fontSize, float x, float y, std::string fontPath = DEFAULT_FONT);

    void Resize();
    bool IsTransparent();

	void SetSpacing(float spacing);
	float GetSpacing();

	// Change the text on this component
    void SetText(std::string text);
private:
    glm::vec2 getUVfromChar(const char c);
    void generateVertices();

    std::string _text;
    float       _fontSize;
	int			_spacing;
};