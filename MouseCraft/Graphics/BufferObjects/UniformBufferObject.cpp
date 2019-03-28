#include "UniformBufferObject.h"

UniformBufferObject::UniformBufferObject() {
	glGenBuffers(1, &_id);
}

UniformBufferObject::~UniformBufferObject() {
	glDeleteBuffers(1, &_id);
}

void UniformBufferObject::bind(int bindingPoint) {
	glBindBuffer(GL_UNIFORM_BUFFER, _id);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _id);
}

void UniformBufferObject::unbind(int bindingPoint) {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, 0);
}

GLuint UniformBufferObject::getID() {
	return _id;
}