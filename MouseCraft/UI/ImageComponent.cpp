#include "ImageComponent.h"

ImageComponent::ImageComponent(std::string imagePath, float width, float height, float x, float y) :
    UIComponent(width, height, x, y), _imagePath(imagePath) {

    //aspectRatio = float(texture.width) / texture.height;
    color = Color(1, 1, 1);
}

bool ImageComponent::IsTransparent() { return true; }

std::string ImageComponent::GetImagePath() {
	return _imagePath;
}

void ImageComponent::SetImagePath(std::string path) {
	_imagePath = path;
}

void ImageComponent::setupModels() {
	UIComponent::setupModels();
	models[0]->setTexture(new std::string(_imagePath));
}