#include <gshader.h>

#include <iostream>

#include <GL/glew.h>


GShader::GShader(GLenum shaderType, const char source[]) :
	shaderId{glCreateShader(shaderType)}
{
	if (source != nullptr) {
		setSource(source);
		compile();
	}
}

GShader::~GShader()
{
	glDeleteShader(shaderId);
}

void GShader::setSource(const char source[])
{
	glShaderSource(shaderId, 1, &source, NULL);
}

void GShader::compile()
{
	glCompileShader(shaderId);

	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shaderId, 512, NULL, buffer);
		std::cout << buffer << std::endl;

		// TODO throw exception
		std::exit(EXIT_FAILURE);
	}
}

GLuint GShader::getShaderId() const
{
	return shaderId;
}
