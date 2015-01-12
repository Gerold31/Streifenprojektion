#include "camerafigure.h"

#include <memory>

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::make_unique;

#define GLSL(src) "#version 150 core\n" #src


const char vertexSource[] = GLSL(
			in vec3 position;
			in vec3 color;
			out vec3 Color;
			uniform mat4 view;
			uniform mat4 model;
			void main() {
				Color = color;
				gl_Position = view * model * vec4(position, 1.0);
			}
);

const char fragmentSource[] = GLSL(
			in vec3 Color;
			out vec4 outColor;
			void main() {
				outColor = vec4(Color, 1.0);
			}
);

CameraFigure::CameraFigure()
{
}

CameraFigure::~CameraFigure()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void CameraFigure::draw(const glm::mat4 &view)
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
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

		GLint colAttrib = glGetAttribLocation(shaderProgram->getProgramId(), "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

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
	glDrawArrays(GL_LINES, 0, 16);
}

void CameraFigure::setConfiguration(float fl, float ratio, float size)
{
	this->focalLength = fl;
	this->ratio = ratio;
	this->size = size;
	shouldUpload = true;
}

void CameraFigure::uploadData()
{
	float vertecies[] = {
		0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f,
		ratio*size*1.0f, size*1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,
		0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f,
		ratio*size*1.0f, size*-1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,
		0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f,
		ratio*size*-1.0f, size*1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,
		0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f,
		ratio*size*-1.0f, size*-1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,

		ratio*size*1.0f, size*1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,
		ratio*size*1.0f, size*-1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,

		ratio*size*1.0f, size*-1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,
		ratio*size*-1.0f, size*-1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,

		ratio*size*-1.0f, size*-1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,
		ratio*size*-1.0f, size*1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,

		ratio*size*-1.0f, size*1.0f, size*focalLength, 0.5f, 1.0f, 0.5f,
		ratio*size*1.0f, size*1.0f, size*focalLength, 0.5f, 1.0f, 0.5f
	};
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 16, vertecies, GL_STATIC_DRAW);
}
