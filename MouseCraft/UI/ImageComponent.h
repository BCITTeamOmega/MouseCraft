#pragma once

#include "UIComponent.h"

/**
Type of UIComponent that displays an image file
*/
class ImageComponent : public UIComponent {
public:
    ImageComponent(std::string imagePath, float width, float height, float x, float y);

    bool IsTransparent();
private:
    std::string _imagePath;
};