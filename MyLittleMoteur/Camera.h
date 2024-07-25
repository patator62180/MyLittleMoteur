#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
	Camera();

	glm::vec3 Position;

	void Rotate(float pitch, float yaw, float roll);

	float GetPitch();
	float GetYaw();
	float GetRoll();

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

	glm::vec3 forward;
	glm::vec3 left;
	glm::vec3 up;

	float pitch;
	float yaw;
	float roll;

	float zoom;

	void UpdateValues();
	void DebugVec3(glm::vec3 vector);
};
