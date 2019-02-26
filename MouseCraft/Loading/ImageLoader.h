#pragma once
#include "../Graphics/Image.h"
#include <string>

class ImageLoader {
public:
	static Image* loadImage(std::string filename);
};