#include "ElementBufferObject.h"

using std::vector;

ElementBufferObject::ElementBufferObject() {
	glGenBuffers(1, &_id);
}

ElementBufferObject::~ElementBufferObject() {
	glDeleteBuffers(1, &_id);
}

void ElementBufferObject::buffer(vector<GLuint>& elements) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		elements.size() * sizeof(GLuint),
		static_cast<void*>(&elements[0]),
		GL_STATIC_DRAW
	);
}

GLuint ElementBufferObject::getID() {
	return _id;
}

void ElementBufferObject::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void ElementBufferObject::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}