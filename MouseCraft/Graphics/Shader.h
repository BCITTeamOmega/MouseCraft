#pragma once
#include "../GL/glad.h"
#include "glm/glm.hpp"
#include <string>

class Shader {
public:
	Shader();
	Shader(std::string vertSrc, std::string fragSrc);
	bool compile();
	GLuint getProgram();
	void setUniformMatrix(std::string name, glm::mat4 matrix);
	void setUniformVec3(std::string name, glm::vec3 vector);
private:
	void printShaderError(GLuint shader);
	void printProgramError(GLuint program);
	std::string vertSrc;
	std::string fragSrc;
	GLuint _program;
};