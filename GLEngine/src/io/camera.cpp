#include "glpch.h"
#include "camera.h"

Camera::Camera(const glm::vec3& position)
	:	CameraPosition(position), 
		WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), 
		m_Yaw(-90.0f), 
		m_Pitch(0.0f), 
		Speed(2.5f), 
		Zoom(45.0f), 
		CameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	UpdateCameraVectors();
}

void Camera::UpdateCameraDirection(f64 dx, f64 dy)
{
	m_Yaw += dx;
	m_Pitch += dy;

	if (m_Pitch > 89.0f) {
		m_Pitch = 89.0f;
	}
	else if (m_Pitch < -89.0f) {
		m_Pitch = -89.0f;
	}
	UpdateCameraVectors();
}

void Camera::UpdateCameraPosition(CameraDirection dir, f64 dt)
{
	f32 velocity = (f32)dt * Speed;

	switch (dir)
	{
	case CameraDirection::FORWARD:
		CameraPosition += CameraFront * velocity;
		break;
	case CameraDirection::BACKWARD:
		CameraPosition -= CameraFront * velocity;
		break;
	case CameraDirection::LEFT:
		CameraPosition -= CameraRight * velocity;
		break;
	case CameraDirection::RIGHT:
		CameraPosition += CameraRight * velocity;
		break;
	case CameraDirection::UP:
		CameraPosition += CameraUp * velocity;
		break;
	case CameraDirection::DOWN:
		CameraPosition -= CameraUp * velocity;
		break;
	default:
		break;
	}
}

void Camera::UpdateCameraPositionByVector(const glm::vec3& vec, f64 dt)
{
	CameraPosition = vec;
}

void Camera::UpdateCameraPositionByValue(CameraDirection dir, f32 value, f64 deltaTime)
{
	f32 velocity = value * Speed * deltaTime;

	switch (dir)
	{
	case CameraDirection::FORWARD:
		CameraPosition.z += velocity;
		break;
	case CameraDirection::BACKWARD:
		CameraPosition.z -= velocity;
		break;
	case CameraDirection::LEFT:
		CameraPosition.x -= velocity;
		break;
	case CameraDirection::RIGHT:
		CameraPosition.x += velocity;
		break;
	case CameraDirection::UP:
		CameraPosition.y += velocity;
		break;
	case CameraDirection::DOWN:
		CameraPosition.y -= velocity;
		break;
	default:
		break;
	}
}

void Camera::UpdateCameraZoom(f64 dy)
{
	if (Zoom >= 1.0f && Zoom <= 60.0f) {
		Zoom -= dy;
	}
	else if (Zoom < 1.0f) {
		Zoom = 1.0f;
	}
	else {
		Zoom = 60.0f;
	}
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(CameraPosition, CameraPosition + CameraFront, CameraUp);
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)); 
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	CameraFront = glm::normalize(direction);

	CameraRight = glm::normalize(glm::cross(CameraFront, WorldUp));
	CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));
}
