#include "GLTextureArray.h"
#include "RenderUtil.h"
GLTextureArray::GLTextureArray(int width, int height, int layers, int mipmapLevels, GLuint storageFormat)
: _width(width), _height(height), _layers(layers), _mipmaps(mipmapLevels) {
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, _mipmaps, storageFormat, _width, _height, _layers);
	RenderUtil::checkGLError("glTexStorage3D");
	if (mipmapLevels == 1) {
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

GLTextureArray::~GLTextureArray() {
	glDeleteTextures(1, &_id);
}

void GLTextureArray::setImage(int layer, Image& image, GLuint inputType) {
	glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
	RenderUtil::checkGLError("glBindTexture");
	GLuint inputFormat = 0;
	GLuint depth = image.getChannels();
	switch (depth) {
	case 1:
		inputFormat = GL_RED;
		break;
	case 2:
		inputFormat = GL_RG;
		break;
	case 3:
		inputFormat = GL_RGB;
		break;
	case 4:
		inputFormat = GL_RGBA;
		break;
	}
	if (!inputFormat) {
		throw "Invalid number of channels in image";
	}
	glTexSubImage3D(
		GL_TEXTURE_2D_ARRAY,
		0,
		0,
		0,
		layer,
		image.getWidth(),
		image.getHeight(),
		1,
		inputFormat,
		inputType,
		image.getData()
	);
	RenderUtil::checkGLError("glTexSubImage3D");
}

void GLTextureArray::bind(GLenum slot) {
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
	RenderUtil::checkGLError("_texture->getImage");
}

void GLTextureArray::genMipmaps() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void GLTextureArray::unbind(GLenum slot) {
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	RenderUtil::checkGLError("_texture->getImage");
}

int GLTextureArray::getWidth() {
	return _width;
}

int GLTextureArray::getHeight() {
	return _height;
}

int GLTextureArray::getLayers() {
	return _layers;
}

GLuint GLTextureArray::getID() {
	return _id;
}