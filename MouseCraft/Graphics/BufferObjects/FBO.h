#pragma once
#include "../../GL/glad.h"
#include "../GLTexture.h"
#include <vector>
class FBO {
public:
	FBO(int width, int height, std::vector<GLTexture*>& textures);
	~FBO();
	void attachBuffers(std::vector<GLTexture*>& buffers);
	void bind();
	void unbind();
	void buffer(GLuint attachment, GLTexture& texture);
	GLuint getID();
private:
	GLuint _id;
	GLuint _rbo;
	int _width;
	int _height;
};