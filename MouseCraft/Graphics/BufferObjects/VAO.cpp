#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &_id);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &_id);
}

GLuint VAO::getID() {
	return _id;
}

void VAO::setBuffer(int id, VBO& vbo) {
	bind();
	vbo.bind();
	glEnableVertexAttribArray(id);
	glVertexAttribPointer(id, vbo.getComponentsPerElement(), GL_FLOAT, GL_FALSE, 0, 0);
	_vbos[id] = &vbo;
}

void VAO::unsetBuffer(int buffID) {
	glDisableVertexArrayAttrib(_id, buffID);
	_vbos.erase(buffID);
}

void VAO::setElementBuffer(EBO& ebo) {
	bind();
	ebo.bind();
	_ebo = &ebo;
}

void VAO::bind() {
	glBindVertexArray(_id);
}

void VAO::unbind() {
	glBindVertexArray(0);
}