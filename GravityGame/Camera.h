#pragma once
#include <glm.hpp>
#include <GLFW/glfw3.h>
#include <ext/matrix_transform.hpp>
#include <ext/matrix_clip_space.hpp>

const float YAW =			-90.0f;
const float PITCH =			0.0f;
const float SPEED =			250.0f;
const float SENSITIVITY =	0.01;
const float FOV =			45.0f;
enum CameraMovementDirection {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
private:
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewProj;

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;

	float yaw;
	float pitch;
	float speed;
	float mouseSensitivity;
	float fov;

private:
public:
	Camera();
	void update();
	inline const glm::mat4& getView() const { return view; }
	inline const glm::mat4& getProj() const { return proj; }
	inline const glm::mat4& getViewProj() const { return viewProj; }
	inline const glm::vec3& getPosition() const { return Position; } //debug

	void translateCamera(CameraMovementDirection direction, float deltaTime);
	void rotateCamera(float xoffset, float yoffset);
	void zoomCamera(float yoffset);
	void resetCamera();
	void rotateScene(float direction, float deltaTime);
};