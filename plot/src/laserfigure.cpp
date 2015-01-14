#include "laserfigure.h"

#include <memory>

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::make_unique;

#define GLSL(src) "#version 150 core\n" #src


const char vertexSource[] = GLSL(
			in vec4 position;
			in vec3 color;
			out vec3 Color;
			uniform mat4 view;
			uniform mat4 model;
			void main() {
				Color = color;
				gl_Position = view * model * position;
			}
);

const char fragmentSource[] = GLSL(
			in vec3 Color;
			out vec4 outColor;
			void main() {
				outColor = vec4(Color, 0.3);
			}
);

LaserFigure::LaserFigure()
{
}

LaserFigure::~LaserFigure()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void LaserFigure::draw(const glm::mat4 &view)
{
	if (first) {
		first = false;
		// Create shader
		vertexShader = make_unique<GShader>(GL_VERTEX_SHADER, vertexSource);
		fragmentShader = make_unique<GShader>(GL_FRAGMENT_SHADER, fragmentSource);
		// Create shader program
		shaderProgram = make_unique<GShaderProgram>();
		shaderProgram->attachShader(*vertexShader);
		shaderProgram->attachShader(*fragmentShader);
		shaderProgram->bindFragDataLocation(0, "outColor");
		shaderProgram->link();

		// Create Vertex Array Object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Create a Vertex Buffer Object and copy the vertex data to it
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		uploadData();

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(shaderProgram->getProgramId(), "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

		GLint colAttrib = glGetAttribLocation(shaderProgram->getProgramId(), "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));

		// Get uniform locations
		uniView = shaderProgram->getUniformLocation("view");
		uniModel = shaderProgram->getUniformLocation("model");
	}

	shaderProgram->use();

	glBindVertexArray(vao);

	// Upload
	if (shouldUpload) {
		uploadData();
		shouldUpload = false;
	}

	// Set up uniforms
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transformation));

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void LaserFigure::uploadData()
{
	float vertecies[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.5f,
		0.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.5f, 0.5f
	};
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 3, vertecies, GL_STATIC_DRAW);
}
