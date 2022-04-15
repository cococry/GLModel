#pragma once

#include "core/coredef.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	Camera(const glm::vec3& position);

	void UpdateCameraDirection(f64 dx, f64 dy);
	void UpdateCameraPosition(CameraDirection dir, f64 dt);
	void UpdateCameraPositionByVector(const glm::vec3& vec, f64 dt);
	void UpdateCameraPositionByValue(CameraDirection dir, f32 value, f64 deltaTime);
	void UpdateCameraZoom(f64 dy);

	glm::mat4 GetViewMatrix();

	glm::vec3 CameraPosition;
	glm::vec3 CameraFront;
	glm::vec3 CameraUp;
	glm::vec3 CameraRight;

	glm::vec3 WorldUp;

	f32 Speed, Zoom;
private:
	void UpdateCameraVectors();

	f32 m_Yaw, m_Pitch;
};