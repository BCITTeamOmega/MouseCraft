#include "ImageComponent.h"

ImageComponent::ImageComponent(std::string imagePath, float width, float height, float x, float y) :
    UIComponent(width, height, x, y), _imagePath(imagePath) {

	// TODO correct texture loading once renderer built
	//texture = AssetLoader::loadTexture(imagePath);
    //aspectRatio = float(texture.width) / texture.height;
    color = {1,1,1,1};

	generateBuffers();
}

bool ImageComponent::IsTransparent() { return true; }