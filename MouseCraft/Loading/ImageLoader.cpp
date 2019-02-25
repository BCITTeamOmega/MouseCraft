#include "ImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

using std::string;

Image* ImageLoader::loadImage(string filename) {
	int width;
	int height;
	int channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	if (!data) {
		std::cerr << "Problem opening file " << filename << std::endl;
		std::cerr << "Reason: " << stbi_failure_reason() << std::endl;
		return nullptr;
	}

	GLuint format = GL_RGBA;
	switch (channels) {
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}
	return new Image(data, width, height, format);
}