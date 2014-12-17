#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <gshader.h>
#include <gshaderprogram.h>
#include <point.h>


class PointCloud
{
public:
	PointCloud(const std::vector<Point> &points);
	~PointCloud();
	void addPoint(float x, float y, float z, float r, float g, float b);
	void draw(const glm::mat4 &view);
private:
	void uploadData();

	std::vector<Point> points;
	GShader vertexShader, fragmentShader;
	GShaderProgram shaderProgram;
	GLint uniView, uniProj;
	GLuint vao;
	GLuint vbo;
};

#endif // POINTCLOUD_H
