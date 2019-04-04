#pragma once
#include "../../GL/glad.h"
#include "../RenderUtil.h"

class UniformBufferObject {
public:
	UniformBufferObject();
	~UniformBufferObject();
	void bind(int bindingPoint);
	void unbind(int bindingPoint);
	template<typename T>
	void buffer(T& data, int count = 1) {
		glBindBuffer(GL_UNIFORM_BUFFER, _id);
		size_t size = sizeof(data) * count;
		if (_allocated != size) {
			glBufferData(GL_UNIFORM_BUFFER, size, &data, GL_STATIC_DRAW);
			_allocated = size;
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, size, &data);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	GLuint getID();
private:
	GLuint _id;
	size_t _allocated;
};