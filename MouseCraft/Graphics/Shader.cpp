#include "Shader.h"
#include "../Loading/TextLoader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using std::cerr;
using std::endl;
using std::string;
using glm::mat4;
using glm::vec3;
using glm::vec4;
using glm::value_ptr;

Shader::Shader(string name, string vertSrc, string fragSrc) :
	_name(name),
	vertSrc(vertSrc),
	fragSrc(fragSrc),
	_program(0) {}

Shader::Shader() : _program(0) {}

bool Shader::compile() {
	const GLchar* vertSrcCStr = vertSrc.c_str();
	const GLchar* fragSrcCStr = fragSrc.c_str();
	int status = 0;

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertShader, 1, &vertSrcCStr, NULL);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if (status == 0) {
		printShaderError(vertShader);
		return false;
	}

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSrcCStr, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (status == 0) {
		printShaderError(fragShader);
		return false;
	}

	_program = glCreateProgram();
	glAttachShader(_program, vertShader);
	glAttachShader(_program, fragShader);
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == 0) {
		printProgramError(_program);
		return false;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	return true;
}

void Shader::printShaderError(GLuint shader) {
	GLint logLen;
	GLchar* logText;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
	logText = (char*)malloc(logLen * sizeof(GLchar));
	glGetShaderInfoLog(shader, logLen, &logLen, logText);
	cerr << "Shader Compile Error(s) in Shader \"" << _name << "\": " << endl
		<< "==================" << endl
		<< logText
		<< "==================" << endl;
	free(logText);
}

void Shader::printProgramError(GLuint program) {
	int logLen;
	char* logText;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
	logText = (char*)malloc(logLen * sizeof(GLchar));
	glGetProgramInfoLog(program, logLen, &logLen, logText);
	cerr << "Shader Program Link Error(s) in Shader \"" << _name << "\": " << endl
		<< "==================" << endl
		<< logText
		<< "==================" << endl;
	free(logText);
}

GLuint Shader::getProgram() {
	return _program;
}

void Shader::setUniformMatrix(string name, mat4 matrix) {
	const char* cstr = name.c_str();
	GLint pos = glGetUniformLocation(_program, cstr);
	glUniformMatrix4fv(pos, 1, GL_FALSE, value_ptr(matrix));
}

void Shader::setUniformVec3(string name, vec3 vector) {
	const char* cstr = name.c_str();
	GLint pos = glGetUniformLocation(_program, cstr);
	glUniform3f(pos, vector.r, vector.g, vector.b);
}

void Shader::setUniformVec4(string name, vec4 vector) {
	const char* cstr = name.c_str();
	GLint pos = glGetUniformLocation(_program, cstr);
	glUniform4f(pos, vector.r, vector.g, vector.b, vector.a);
}

void Shader::setUniformTexture(string name, GLuint index) {
	const char* cstr = name.c_str();
	GLint pos = glGetUniformLocation(_program, cstr);
	glUniform1i(pos, index);
}

void Shader::setUniformInt(string name, GLint value) {
	const char* cstr = name.c_str();
	GLint pos = glGetUniformLocation(_program, cstr);
	glUniform1i(pos, value);
}

void Shader::setUniformFloat(string name, GLfloat value) {
	const char* cstr = name.c_str();
	GLint pos = glGetUniformLocation(_program, cstr);
	glUniform1f(pos, value);
}

void Shader::setBindingPoint(string name, GLint value) {
	const char* cstr = name.c_str();
	GLuint index = glGetUniformBlockIndex(_program, cstr);
	glUniformBlockBinding(_program, index, value);
}