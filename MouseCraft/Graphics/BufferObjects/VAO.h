#pragma once
#include "../../GL/glad.h"
#include "VBO.h"
#include "EBO.h"
#include <map>
class VAO {
public:
	VAO();
	~VAO();
	GLuint getID();
	void setBuffer(int id, VBO& vbo);
	void unsetBuffer(int id);
	void setElementBuffer(EBO& ebo);
	void bind();
	void unbind();
private:
	GLuint _id;
	std::map<int, VBO*> _vbos;
	EBO* _ebo;
};