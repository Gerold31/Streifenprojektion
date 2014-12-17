#ifndef GSHADERPROGRAM_H
#define GSHADERPROGRAM_H

#include <GL/glew.h>

#include <gshader.h>


class GShaderProgram
{
public:
	GShaderProgram();
	~GShaderProgram();
	void attachShader(const GShader &shader);
	void bindFragDataLocation(GLuint colorNumber, const char name[]);
	GLuint getUniformLocation(const char name[]);
	void link();
	void use();

	GLuint getProgramId() const;
private:
	GShaderProgram(const GShaderProgram&) = delete;
	GShaderProgram& operator=(const GShaderProgram&) = delete;

	GLuint programId;
};

#endif // GSHADERPROGRAM_H
