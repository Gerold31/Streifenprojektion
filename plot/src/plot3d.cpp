#include <plot3d.h>

#include <iostream>
#include <memory>
#include <mutex>

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <pointcloud.h>

using glm::inverse;
using glm::mat4;
using std::lock_guard;
using std::make_shared;
using std::mutex;
using std::shared_ptr;


void Plot3D::addDrawable(std::shared_ptr<Drawable> drawable)
{
	lock_guard<mutex> lock{mutexDrawables};
	newDrawables.push_back(drawable);
}

void Plot3D::addPoint(float x, float y, float z, float r, float g, float b)
{
	lock_guard<mutex> lock{mutexPoints};
	newPoints.emplace_back(x, y, z, r, g, b);
}

void Plot3D::show(int resolutionX, int resolutionY)
{
	// create context
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(resolutionX, resolutionY, "3D-Plot",
										  nullptr, nullptr);

	glfwMakeContextCurrent(window);

	// init glew
	glewExperimental = GL_TRUE;
	glewInit();

	// Initialize OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create Drawables
	{
		lock_guard<mutex> lock{mutexPoints};
		pointCloud = make_shared<PointCloud>(newPoints);
		newPoints.clear();
	}
	{
		lock_guard<mutex> lock{mutexDrawables};
		drawables.push_back(pointCloud);
	}

	// Init view position and perspective
	cam = Camera{};

	// Store last cursor position and time
	double cursorX, cursorY;
	double time = glfwGetTime();
	glfwGetCursorPos(window, &cursorX, &cursorY);

	while (!glfwWindowShouldClose(window))
	{
		// Update time
		double dt = time;
		time = glfwGetTime();
		dt = time - dt;

		// Read input events
		glfwPollEvents();

		// Handle input
		{

			glm::mat4& camTr = cam.getTransformation();
			// Update cam position
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
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
				movement[1] += -1.0f * dt;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
				movement[1] += -1.0f * dt;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				movement[1] += 1.0f * dt;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				movement *= 2;
			}
			movement *= moveSpeed;
			camTr = glm::translate(camTr, movement);
			// Update cam rotation
			glm::vec3 rotation{0.0f, 0.0f, 0.0f};
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				rotation[2] += -1.0f * dt;
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				rotation[2] += 1.0f * dt;
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS
					|| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
				double newCursorX, newCursorY;
				glfwGetCursorPos(window, &newCursorX, &newCursorY);
				glfwSetCursorPos(window, cursorX, cursorY);
				rotation[0] = - ( newCursorX - cursorX ) * 0.001;
				rotation[1] = - ( newCursorY - cursorY ) * 0.001;
			} else {
				glfwGetCursorPos(window, &cursorX, &cursorY);
			}
			rotation *= sensibility;
			camTr = glm::rotate(camTr, (float) rotation[0], glm::vec3{0.0f, 1.0f, 0.0f});
			camTr = glm::rotate(camTr, (float) rotation[1], glm::vec3{1.0f, 0.0f, 0.0f});
			camTr = glm::rotate(camTr, (float) rotation[2], glm::vec3{0.0f, 0.0f, 1.0f});
			// Other key handles
			static bool acceptR = true, acceptF = true, acceptPeriod = true;
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
				if (acceptR) {
					moveSpeed *= 2;
					acceptR = false;
				}
			} else {
				acceptR = true;
			}
			if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
				if (acceptF) {
					moveSpeed /= 2;
					acceptF = false;
				}
			} else {
				acceptF = true;
			}
			if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
				if (acceptPeriod) {
					pointCloud->setOptions(PointCloud::COLOR_DISTANCE);
					acceptPeriod = false;
				}
			} else {
				acceptPeriod = true;
			}
		}

		// Add new points
		{
			lock_guard<mutex> lock{mutexPoints};
			for (Point p : newPoints) {
				pointCloud->addPoint(p.x, p.y, p.z, p.r, p.g, p.b);
			}
			newPoints.clear();
		}
		// Add new drawables
		{
			lock_guard<mutex> lock{mutexDrawables};
			for (shared_ptr<Drawable> &drawable : newDrawables) {
				drawables.push_back(drawable);
			}
			newDrawables.clear();
		}

		// Render
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			mat4 projection = cam.getProjection(resolutionX, resolutionY);
			for (shared_ptr<Drawable> &drawable : drawables) {
				drawable->draw(projection);
			}

			glfwSwapBuffers(window);
		}
	}

	pointCloud = nullptr;
	glfwTerminate();
}
