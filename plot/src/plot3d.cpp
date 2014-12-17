#include <plot3d.h>

#include <iostream>

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <pointcloud.h>


void Plot3D::addPoint(float x, float y, float z, float r, float g, float b)
{
	points.emplace_back(x, y, z, r, g, b);
	if (pointCloud != nullptr) {
		pointCloud->addPoint(x, y, z, r, g, b);
	}
}

void Plot3D::show()
{
	// create context
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	// init glew
	glewExperimental = GL_TRUE;
	glewInit();

	// Initialize OpenGL
	glEnable(GL_DEPTH_TEST);

	pointCloud = new PointCloud(points);

	// Init view position and perspective
	//glm::vec3 viewPos = {4.0f, 1.0f, 2.0f};
	glm::mat4 view = glm::inverse(glm::lookAt(
				glm::vec3(4.0f, 1.0f, 2.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)));
	glm::mat4 proj = glm::perspective(0.8f, 800.0f / 600.0f, 0.01f, 100.0f);

	// Store last cursor position and time
	double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	double time = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		// Update time
		double dt = time;
		time = glfwGetTime();
		dt = time - dt;

		// Read input events
		glfwPollEvents();

		// Update view position
		glm::vec3 movement{0.0f, 0.0f, 0.0f};
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			movement[2] += -1.0f * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			movement[2] += 1.0f * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			movement[0] += -1.0f * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			movement[0] += 1.0f * dt;
		}
		//if (glfwGetKey(window, GLFW_MOD_SHIFT) == GLFW_PRESS) {
		//	movement[1] += -1.0f * dt;
		//}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			movement[1] += 1.0f * dt;
		}
		view = glm::translate(view, movement);
		// Update view rotation
		glm::vec3 rotation{0.0f, 0.0f, 0.0f};
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			rotation[2] += -1.0f * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			rotation[2] += 1.0f * dt;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
			double newCursorX, newCursorY;
			glfwGetCursorPos(window, &newCursorX, &newCursorY);
			glfwSetCursorPos(window, cursorX, cursorY);
			rotation[0] = - ( newCursorX - cursorX ) * 0.001;
			rotation[1] = - ( newCursorY - cursorY ) * 0.001;
		} else {
			glfwGetCursorPos(window, &cursorX, &cursorY);
		}
		view = glm::rotate(view, (float) rotation[0], glm::vec3{0.0f, 1.0f, 0.0f});
		view = glm::rotate(view, (float) rotation[1], glm::vec3{1.0f, 0.0f, 0.0f});
		view = glm::rotate(view, (float) rotation[2], glm::vec3{0.0f, 0.0f, 1.0f});

		// render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pointCloud->draw(proj * glm::inverse(view));

		glfwSwapBuffers(window);
	}

	delete pointCloud;

	glfwTerminate();
}
