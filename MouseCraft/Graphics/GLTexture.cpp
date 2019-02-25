#include "GLTexture.h"
GLTexture::GLTexture() {
	glGenTextures(1, &_id);
}

GLTexture::~GLTexture() {
	glDeleteTextures(1, &_id);
}

void GLTexture::setImage(Image& image, bool mipmap) {
	glBindTexture(GL_TEXTURE_2D, _id);
	GLuint format = image.getFormat();
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		image.getWidth(),
		image.getHeight(),
		0,
		format,
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