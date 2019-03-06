#include "FBO.h"
FBO::FBO() {
	glGenFramebuffers(1, &_id);
}

FBO::~FBO() {
	glDeleteFramebuffers(1, &_id);
}

void FBO::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void FBO::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::buffer(GLuint attachment, GLTexture texture) {
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.getID(), 0);
}

GLuint FBO::getID() {
	return _id;
}