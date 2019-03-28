#include "VBO.h"

using std::vector;

VBO::VBO(int componentsPerElement) : _componentsPerElement(componentsPerElement) {
	glGenBuffers(1, &_id);
}

VBO::~VBO() {
	glDeleteBuffers(1, &_id);
}

GLuint VBO::getID() {
	return _id;
}

void VBO::buffer(vector<GLfloat>& values) {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(
		GL_ARRAY_BUFFER,
		values.size() * sizeof(GLfloat),
		static_cast<void*>(&values[0]),
		GL_STATIC_DRAW
	);
}

int VBO::getComponentsPerElement() {
	return _componentsPerElement;
}

void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VBO::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}