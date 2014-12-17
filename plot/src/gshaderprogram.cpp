#include <gshaderprogram.h>

#include <GL/glew.h>

#include <gshader.h>


GShaderProgram::GShaderProgram() :
	programId{glCreateProgram()}
{
}

GShaderProgram::~GShaderProgram()
{
	glDeleteProgram(programId);
}

void GShaderProgram::attachShader(const GShader &shader)
{
	glAttachShader(programId, shader.getShaderId());
}

void GShaderProgram::bindFragDataLocation(GLuint colorNumber, const char name[])
{
	glBindFragDataLocation(programId, colorNumber, name);
}

GLuint GShaderProgram::getUniformLocation(const char name[])
{
	return glGetUniformLocation(programId, name);
}

void GShaderProgram::link()
{
	glLinkProgram(programId);
}

void GShaderProgram::use()
{
	glUseProgram(programId);
}

GLuint GShaderProgram::getProgramId() const
{
	return programId;
}
