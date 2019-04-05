#pragma once
#include "../../GL/glad.h"
#include <vector>
class VertexBufferObject {
public:
	VertexBufferObject(int componentsPerElement);
	~VertexBufferObject();
	GLuint getID();
	int getComponentsPerElement();
	void buffer(std::vector<GLfloat>& values);
	void bind();
	void unbind();
private:
	GLuint _id;
	int _componentsPerElement;
};