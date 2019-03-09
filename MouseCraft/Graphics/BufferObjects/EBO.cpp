#include "EBO.h"

using std::vector;

EBO::EBO() {
	glGenBuffers(1, &_id);
}

EBO::~EBO() {
	glDeleteBuffers(1, &_id);
}

void EBO::buffer(vector<GLuint>& elements) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		elements.size() * sizeof(GLuint),
		static_cast<void*>(&elements[0]),
		GL_STATIC_DRAW
	);
}

GLuint EBO::getID() {
	return _id;
}

void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void EBO::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}