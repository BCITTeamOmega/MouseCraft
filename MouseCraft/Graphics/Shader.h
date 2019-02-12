#pragma once
#include <string>
#include <GLEW/glew.h>

using std::string;

class Shader {
public:
	Shader();
	Shader(string vertSrc, string fragSrc);
	bool compile();
	GLuint getProgram();
private:
	void printShaderError(GLuint shader);
	void printProgramError(GLuint program);
	string vertSrc;
	string fragSrc;
	GLuint _program;
};