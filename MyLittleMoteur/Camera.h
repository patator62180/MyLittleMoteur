#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
	Camera();

	glm::vec3 Position;

	float GetPitch();
	float GetYaw();

	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetLeft();

	glm::mat4 GetLookAt();
	glm::mat4 GetProjection();

	void ProcessInput(GLFWwindow* window, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessMouseScroll(float yoffset);
private:
	const float cameraSpeed = 5.0f;
	const float mouseSensitivity = 0.1f;
	const float minZoom = 1.0f;
	const float maxZoom = 45.0f;
	const glm::vec3 world_up = glm::vec3(0.0, 1.0, 0.0);
	const glm::vec3 world_right = glm::vec3(1.0, 0.0, 0.0);
	const glm::vec3 world_forward = glm::vec3(0.0, 0.0, 1.0);

	glm::vec3 forward;
	glm::vec3 left;
	glm::vec3 up;

	float pitch;
	float yaw;

	float zoom;

	void UpdateValues();
	void DebugVec3(glm::vec3 vector);
};
