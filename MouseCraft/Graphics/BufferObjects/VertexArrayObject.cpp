#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject() {
	glGenVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject() {
	glDeleteVertexArrays(1, &_id);
}

GLuint VertexArrayObject::getID() {
	return _id;
}

void VertexArrayObject::setBuffer(int id, VertexBufferObject& vbo, int offset) {
	bind();
	vbo.bind();
	glEnableVertexAttribArray(id);
	glVertexAttribPointer(id, vbo.getComponentsPerElement(), GL_FLOAT, GL_FALSE, 0, (void *)offset);
	_vbos[id] = &vbo;
}

void VertexArrayObject::unsetBuffer(int buffID) {
	bind();
	glEnableVertexAttribArray(0);
	_vbos.erase(buffID);
}

void VertexArrayObject::setElementBuffer(ElementBufferObject& ebo) {
	bind();
	ebo.bind();
	_ebo = &ebo;
}

void VertexArrayObject::bind() {
	glBindVertexArray(_id);
}

void VertexArrayObject::unbind() {
	glBindVertexArray(0);
}