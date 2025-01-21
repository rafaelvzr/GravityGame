#include "Camera.h"

Camera::Camera()
	: Position(glm::vec3(195.76f, 96.61f, -66.68f)), Front(glm::vec3(0.0f,0.0f, -1.0f)), Up(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(YAW), pitch(PITCH), 
	speed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV)
{
	glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), Up);
	proj = glm::perspective(glm::radians(fov), 1024.0f / 768.0f, 0.1f, 1000.0f);
	viewProj = proj * view;
}

void Camera::update()
{
	view = glm::lookAt(Position, glm::vec3(0.0f,0.0f,0.0f), Up);
	proj = glm::perspective(glm::radians(fov), 1024.0f / 768.0f, 0.1f, 1000.0f);
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
	bool up = yoffset >= 0.0f;
	float direction = up ? 1.0f : -1.0f;
	Position += glm::vec3(0.0f, direction, 0.0f);
	if (Position.y > 180.0f)
		Position.y = 180.0f;
	if (Position.y < -180.0f)
		Position.y = -180.0f;
	view = glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), Up);
	viewProj = proj * view;
	return;
	//xoffset *= mouseSensitivity;
	//yoffset *= mouseSensitivity;
	//yaw += xoffset;
	//pitch += yoffset;
	//if (pitch > 89.0f)
	//	pitch = 89.0f;
	//if (pitch < -89.0f)
	//	pitch = -89.0f;
	//glm::vec3 kelber;
	//kelber.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//kelber.y = sin(glm::radians(pitch));
	//kelber.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//Front = glm::normalize(kelber);
	//viewProj = proj * view;
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
	Position = glm::vec3(195.76f, 96.61f, -66.68f);
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = YAW;
	pitch = PITCH;
	view = glm::lookAt(Position, glm::vec3(0.0f, 0.0f,0.0f), Up);
	proj = glm::perspective(glm::radians(FOV), 1024.0f / 768.0f, 0.1f, 100.0f);
	viewProj = proj * view;
}

void Camera::rotateScene(float direction, float deltaTime)
{
	//rotate around (0,0,0)
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(direction * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
	Position = glm::vec3(rotation * glm::vec4(Position, 1.0f));
	view = glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), Up);
	viewProj = proj * view;
}
