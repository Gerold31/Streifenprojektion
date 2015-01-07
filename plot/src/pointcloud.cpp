#include <pointcloud.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLSL(src) "#version 150 core\n" #src


const char vertexSource[] = GLSL(
			in vec3 position;
			in vec3 color;
			out vec3 Color;
			uniform mat4 view;
			uniform mat4 proj;
			void main() {
				Color = color;
				gl_Position = proj * view * vec4(position, 1.0);
			}
);

const char fragmentSource[] = GLSL(
			in vec3 Color;
			out vec4 outColor;
			void main() {
				outColor = vec4(Color, 1.0);
			}
);


PointCloud::PointCloud(const std::vector<Point> &points) :
	vertexShader{GL_VERTEX_SHADER, vertexSource},
	fragmentShader{GL_FRAGMENT_SHADER, fragmentSource},
	points{points}
{
	// Create Shader Program
	shaderProgram.attachShader(vertexShader);
	shaderProgram.attachShader(fragmentShader);
	shaderProgram.bindFragDataLocation(0, "outColor");
	shaderProgram.link();

	// Create Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	uploadData();

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram.getProgramId(), "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram.getProgramId(), "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	// Get uniform locations
	uniView = shaderProgram.getUniformLocation("view");
	uniProj = shaderProgram.getUniformLocation("proj");
}

PointCloud::~PointCloud()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::addPoint(float x, float y, float z, float r, float g, float b)
{
	points.emplace_back(x, y, z, r, g, b);
	shouldUpload = true;
}

void PointCloud::draw(const glm::mat4 &view)
{
	shaderProgram.use();
	glBindVertexArray(vao);

	// upload
	if (shouldUpload) {
		uploadData();
		shouldUpload = false;
	}

	// Set up uniforms
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(glm::mat4{}));

	const size_t vertexCount = points.size();
	glDrawArrays(GL_POINTS, 0, vertexCount);
}

void PointCloud::uploadData()
{
	const size_t vertexCount = points.size();
	glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * vertexCount, points.data(), GL_STATIC_DRAW);
}