#include "VertexBufferObject.h"

using std::vector;

VertexBufferObject::VertexBufferObject(int componentsPerElement) : _componentsPerElement(componentsPerElement) {
	glGenBuffers(1, &_id);
}

VertexBufferObject::~VertexBufferObject() {
	glDeleteBuffers(1, &_id);
}

GLuint VertexBufferObject::getID() {
	return _id;
}

void VertexBufferObject::buffer(vector<GLfloat>& values) {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(
		GL_ARRAY_BUFFER,
		values.size() * sizeof(GLfloat),
		static_cast<void*>(&values[0]),
		GL_STATIC_DRAW
	);
}

int VertexBufferObject::getComponentsPerElement() {
	return _componentsPerElement;
}

void VertexBufferObject::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBufferObject::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}