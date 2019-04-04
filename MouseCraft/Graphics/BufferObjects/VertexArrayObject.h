#pragma once
#include "../../GL/glad.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include <map>
class VertexArrayObject {
public:
	VertexArrayObject();
	~VertexArrayObject();
	GLuint getID();
	void setBuffer(int id, VertexBufferObject& vbo, int offset = 0);
	void unsetBuffer(int id);
	void setElementBuffer(ElementBufferObject& ebo);
	void bind();
	void unbind();
private:
	GLuint _id;
	std::map<int, VertexBufferObject*> _vbos;
	ElementBufferObject* _ebo;
};