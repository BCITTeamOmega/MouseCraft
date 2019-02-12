#pragma once
#include <string>
#include <GLEW/glew.h>

class Shader {
public:
	Shader();
	Shader(std::string vertSrc, std::string fragSrc);
	bool compile();
	GLuint getProgram();
private:
	void printShaderError(GLuint shader);
	void printProgramError(GLuint program);
	std::string vertSrc;
	std::string fragSrc;
	GLuint _program;
};