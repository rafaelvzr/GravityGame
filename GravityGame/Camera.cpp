#include "Camera.h"

Camera::Camera()
	: Position(glm::vec3(0.0f, 0.0f, 3.0f)), Front(glm::vec3(0.0f, 0.0f, -1.0f)), Up(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(YAW), pitch(PITCH), 
	speed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV)
{
	view = glm::lookAt(Position, Position + Front, Up);
	proj = glm::perspective(glm::radians(fov), 1024.0f / 768.0f, 0.1f, 100.0f);
	viewProj = proj * view;
}

void Camera::update()
{
	view = glm::lookAt(Position, Position + Front, Up);
	proj = glm::perspective(glm::radians(fov), 1024.0f / 768.0f, 0.1f, 100.0f);
	viewProj = proj * view;
}

void Camera::translateCamera(CameraMovementDirection direction, float deltaTime)
{
	float velocity = SPEED * deltaTime;
	switch (direction)
	{
	case FORWARD:
		Position += velocity * Front;
		break;
	case BACKWARD:
		Position -= velocity * Front;
		break;
	case LEFT:
		Position -= glm::normalize(glm::cross(Front, Up)) * velocity;
		break;
	case RIGHT:
		Position += glm::normalize(glm::cross(Front, Up)) * velocity;
		break;
	}
	viewProj = proj * view;
}

void Camera::rotateCamera(float xoffset, float yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front = glm::normalize(direction);
	viewProj = proj * view;
}

void Camera::zoomCamera(float yoffset)
{
	fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
	proj = glm::perspective(glm::radians(fov), 1024.0f / 768.0f, 0.1f, 100.0f);
	viewProj = proj * view;
}

void Camera::resetCamera()
{
	//Position = glm::vec3(0.0f, 0.0f, 3.0f);
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = YAW;
	pitch = PITCH;
	view = glm::lookAt(Position, Position + Front, Up);
	proj = glm::perspective(glm::radians(FOV), 1024.0f / 768.0f, 0.1f, 100.0f);
	viewProj = proj * view;
}
