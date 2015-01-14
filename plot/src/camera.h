#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera() = default;

	glm::mat4& getTransformation();
	const glm::mat4& getTransformation() const;
	glm::mat4 getProjection(int resolutionX, int resolutionY) const;
private:
	float fov;
	float nearClip;
	float farClip;
	glm::mat4 transformation;
};


inline Camera::Camera() :
	fov{0.8f},
	nearClip{0.1f},
	farClip{1000.0f},
	transformation{glm::inverse(glm::lookAt(
									glm::vec3(0.0f, 0.0f, 0.0f),
									glm::vec3(0.0f, 0.0f, -1.0f),
									glm::vec3(0.0f, 1.0f, 0.0f)))}
{
}

inline glm::mat4 &Camera::getTransformation()
{
	return transformation;
}

inline const glm::mat4 &Camera::getTransformation() const
{
	return transformation;
}

inline glm::mat4 Camera::getProjection(int resolutionX, int resolutionY) const
{
	float aspect = (float) resolutionX / resolutionY;
	glm::mat4 proj = glm::perspective(fov, aspect, nearClip, farClip);
	return proj * glm::inverse(transformation);
}

#endif // CAMERA_H
