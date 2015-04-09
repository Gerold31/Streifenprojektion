#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <drawable.h>
#include <gshader.h>
#include <gshaderprogram.h>
#include <point.h>


class PointCloud : public Drawable
{
public:
	static const int COLOR_DISTANCE = 1;

	PointCloud(const std::vector<Point> &points);
	virtual ~PointCloud();

	void addPoint(float x, float y, float z, float r, float g, float b);
	void setOptions(int options);
	void setPointPerspectiveSize(bool enable);
	void setPointSize(float size);

	virtual void draw(const glm::mat4 &view) override;
private:
	void uploadData();

	std::vector<Point> points;
	GShader vertexShader, fragmentShader, fragmentShaderEX;
	GShaderProgram shaderProgram, shaderProgramEX;
	GLint uniView, uniProj, uniPointSize, uniPointResize;
	GLuint vao;
	GLuint vbo;
	bool shouldUpload;

	int options = 0;
	float pointSize = 1;
	bool pointPerspectiveSize = true;
};

#endif // POINTCLOUD_H
