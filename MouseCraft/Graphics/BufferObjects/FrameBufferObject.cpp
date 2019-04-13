#include "FrameBufferObject.h"
#include "../RenderUtil.h"
#include <iostream>

using std::vector;

FrameBufferObject::FrameBufferObject(int width, int height, vector<GLTexture*>& textures) : _width(width), _height(height) {
	glGenFramebuffers(1, &_id);
	RenderUtil::checkGLError("glGenFramebuffers");
	glGenRenderbuffers(1, &_rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);
	if (textures.size() > 0) {
		attachBuffers(textures);
	}
}

FrameBufferObject::~FrameBufferObject() {
	glDeleteFramebuffers(1, &_id);
	RenderUtil::checkGLError("glDeleteFramebuffers");
	glDeleteRenderbuffers(1, &_rbo);
}

void FrameBufferObject::attachBuffers(std::vector<GLTexture*>& buffers) {
	Image* img = new Image(NULL, _width, _height); // Temp image so we can use GLtextures
	vector<GLuint> attachments;
	for (int i = 0; i < buffers.size(); i++) {
		GLTexture* b = buffers[i];
		b->setImage(*img, false, GL_RGBA16F, GL_FLOAT);
		GLuint attachment = GL_COLOR_ATTACHMENT0 + i;
		attachments.push_back(attachment);
		// Calls internal function to attach buffer
		buffer(attachment, *b);
	}
	bind();
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
	}
	glDrawBuffers(attachments.size(), &attachments[0]);
	delete img;
}

void FrameBufferObject::bind(GLuint type) {
	glBindFramebuffer(type, _id);
	RenderUtil::checkGLError("glBindFramebuffer");
}

void FrameBufferObject::unbind(GLuint type) {
	glBindFramebuffer(type, 0);
	RenderUtil::checkGLError("glBindFramebuffer");
}

void FrameBufferObject::buffer(GLuint attachment, GLTexture& texture) {
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.getID(), 0);
	RenderUtil::checkGLError("glFrameBufferTexture2D");
}

GLuint FrameBufferObject::getID() {
	return _id;
}

void FrameBufferObject::blit(FrameBufferObject& source, int srcWidth, int srcHeight, GLuint type) {
	source.bind(GL_READ_FRAMEBUFFER);
	bind(GL_DRAW_FRAMEBUFFER);

	GLuint scaling = GL_LINEAR;
	if (type & GL_DEPTH_BUFFER_BIT || type & GL_STENCIL_BUFFER_BIT) {
		scaling = GL_NEAREST;
	}

	glBlitFramebuffer(0, 0, srcWidth, srcHeight, 0, 0, _width, _height, type, scaling);
	RenderUtil::checkGLError("glBlitFramebuffer");

	source.unbind(GL_READ_FRAMEBUFFER);
	unbind(GL_DRAW_FRAMEBUFFER);
}

int FrameBufferObject::getWidth() {
	return _width;
}

int FrameBufferObject::getHeight() {
	return _height;
}