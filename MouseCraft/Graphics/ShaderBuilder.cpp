#include "ShaderBuilder.h"

Shader ShaderBuilder::compile(string vertSrc, string fragSrc) {
	GLuint vertShader = 0;
	GLuint fragShader = 0;

	const GLchar* vertSrcCStr = vertSrc.c_str();
	const GLchar* fragSrcCStr = fragSrc.c_str();
	glShaderSource(vertShader, 1, &vertSrcCStr, NULL);
	glCompileShader(vertShader);
	glShaderSource(fragShader, 1, &fragSrcCStr, NULL);
	glCompileShader(fragShader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	return Shader(program);
}