#ifndef GSHADER_H
#define GSHADER_H

#include <GL/glew.h>

class GShaderProgram;


class GShader
{
public:
	GShader(GLenum shaderType, const char source[] = nullptr);
	~GShader();
	void setSource(const char source[]);
	void compile();

	GLuint getShaderId() const;
private:
	GShader(const GShader&) = delete;
	GShader& operator=(const GShader&) = delete;

	GLuint shaderId;
};

#endif // GSHADER_H
