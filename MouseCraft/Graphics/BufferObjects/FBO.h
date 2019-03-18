#pragma once
#include "../../GL/glad.h"
#include "../GLTexture.h"
class FBO {
public:
	FBO();
	~FBO();
	void bind();
	void unbind();
	void buffer(GLuint attachment, GLTexture& texture);
	GLuint getID();
private:
	GLuint _id;
	GLuint _rbo;
};