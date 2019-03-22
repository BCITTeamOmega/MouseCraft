#pragma once
#include "../GL/glad.h"
#include "Image.h"
class GLTextureArray {
public:
	GLTextureArray(int width, int height, int layers, int mipmapLevels, GLuint storageFormat);
	~GLTextureArray();
	GLuint getID();
	void setImage(
		int layer,
		Image& image,
		GLuint inputType = GL_FLOAT
	);
	void bind(GLenum slot);
	void unbind(GLenum slot);
	void genMipmaps();
	int getWidth();
	int getHeight();
	int getLayers();
private:
	GLuint _id;
	int _layers;
	int _width;
	int _height;
	int _mipmaps;
};