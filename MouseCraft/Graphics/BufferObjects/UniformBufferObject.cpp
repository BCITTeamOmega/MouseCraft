#include "UniformBufferObject.h"

UniformBufferObject::UniformBufferObject() {
	glGenBuffers(1, &_id);
}

UniformBufferObject::~UniformBufferObject() {
	glDeleteBuffers(1, &_id);
}

void UniformBufferObject::bind(int bindingPoint) {
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _id);
}

void UniformBufferObject::unbind(int bindingPoint) {
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, 0);
}

GLuint UniformBufferObject::getID() {
	return _id;
}