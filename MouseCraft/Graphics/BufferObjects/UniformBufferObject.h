#pragma once
#include "../../GL/glad.h"
class UniformBufferObject {
public:
	UniformBufferObject();
	~UniformBufferObject();
	void bind(int bindingPoint);
	void unbind(int bindingPoint);
	template<typename T>
	void buffer(T& data) {
		glBindBuffer(GL_UNIFORM_BUFFER, _id);
		size_t size = sizeof(data);
		if (_allocated != size) {
			glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
			_allocated = size;
		}
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size, &data);
	}
	GLuint getID();
private:
	GLuint _id;
	size_t _allocated;
};