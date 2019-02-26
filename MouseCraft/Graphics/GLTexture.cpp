#include "GLTexture.h"
GLTexture::GLTexture() {
	glGenTextures(1, &_id);
}

GLTexture::~GLTexture() {
	glDeleteTextures(1, &_id);
}

void GLTexture::setImage(Image& image, bool mipmap, GLuint storageFormat) {
	glBindTexture(GL_TEXTURE_2D, _id);
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
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		storageFormat,
		image.getWidth(),
		image.getHeight(),
		0,
		inputFormat,
		GL_UNSIGNED_BYTE,
		image.getData()
	);
	if (mipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void GLTexture::bind(GLenum slot) {
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, _id);
}

void GLTexture::unbind(GLenum slot) {
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint GLTexture::getID() {
	return _id;
}