#ifndef LASERFIGURE_H
#define LASERFIGURE_H

#include <memory>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <drawable.h>
#include <gshader.h>
#include <gshaderprogram.h>


class LaserFigure : public Drawable
{
public:
	LaserFigure();
	virtual ~LaserFigure();

	virtual void draw(const glm::mat4 &view) override;
	void setConfiguration(float fl, float ratio, float size);

	glm::mat4 transformation{};
private:
	void uploadData();

	float focalLength = 1.0f;
	float ratio = 1.2f;
	float size = 2.0f;

	std::unique_ptr<GShader> vertexShader, fragmentShader;
	std::unique_ptr<GShaderProgram> shaderProgram;
	GLint uniModel, uniView;
	GLuint vao;
	GLuint vbo;
	bool shouldUpload = true;
	bool first = true;
};

#endif // LASERFIGURE_H
