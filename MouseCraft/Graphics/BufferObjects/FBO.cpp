#include "FBO.h"
#include "../RenderUtil.h"
#include <iostream>

FBO::FBO() {
	glGenFramebuffers(1, &_id);
	RenderUtil::checkGLError("glGenFramebuffers");
	glGenRenderbuffers(1, &_rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);
}

FBO::~FBO() {
	glDeleteFramebuffers(1, &_id);
	RenderUtil::checkGLError("glDeleteFramebuffers");
	glDeleteRenderbuffers(1, &_rbo);
}

void FBO::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	RenderUtil::checkGLError("glBindFramebuffer");
}

void FBO::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderUtil::checkGLError("glBindFramebuffer");
}

void FBO::buffer(GLuint attachment, GLTexture& texture) {
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.getID(), 0);
	RenderUtil::checkGLError("glFrameBufferTexture2D");
}

GLuint FBO::getID() {
	return _id;
}