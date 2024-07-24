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

	void ProcessInput(GLFWwindow* window);
private:
	const float cameraSpeed = 0.005f;

	glm::vec3 forward;
	glm::vec3 left;
	glm::vec3 up;

	float pitch;
	float yaw;
	float roll;

	void UpdateValues();
};
