#pragma once
#include "../../GL/glad.h"
#include <vector>
class EBO {
public:
	EBO();
	~EBO();
	GLuint getID();
	void buffer(std::vector<GLuint>& elements);
	void bind();
	void unbind();
private:
	GLuint _id;
};