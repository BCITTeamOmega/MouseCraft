#pragma once
#include <string>
#include <GLEW/glew.h>

using std::string;

class Shader {
public:
	Shader(GLuint program);
private:
	GLuint _program;
};