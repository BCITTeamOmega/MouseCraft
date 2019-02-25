#pragma once
#include "../GL/glad.h"
#include "Image.h"
class GLTexture {
public:
	GLTexture();
	~GLTexture();
	GLuint getID();
	void setImage(
		Image& image,
		bool mipmap = true,
		GLuint storageFormat = GL_RGBA16F
	);
	void bind(GLenum slot);
	void unbind(GLenum slot);
private:
	GLuint _id;
};