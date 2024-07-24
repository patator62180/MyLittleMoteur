#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

Camera::Camera()
{
	Position = glm::vec3(0.0, 0.0, 3.0);
	pitch = 0;
	yaw = 0;
	roll = 0;
	forward = glm::vec3(0.0, 0.0, -1.0);
	up = glm::vec3(0.0, 1.0, 0.0);
	left = glm::vec3(-1.0, 0.0, 0.0);

}

void Camera::Rotate(float pitch, float yaw, float roll)
{
	this->pitch += pitch;
	this->yaw += yaw;
	this->roll += roll;
	UpdateValues();
}

float Camera::GetPitch()
{
	return pitch;
}

float Camera::GetYaw()
{
	return yaw;
}

float Camera::GetRoll() 
{
	return roll;
}

glm::vec3 Camera::GetForward()
{
	return forward;
}

glm::vec3 Camera::GetLeft()
{
	return left;
}

glm::vec3 Camera::GetUp()
{
	return up;
}

glm::mat4 Camera::GetLookAt()
{
	return glm::lookAt(Position, Position + forward, up);
}

void Camera::UpdateValues()
{
	//TODO
}


void Camera::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += cameraSpeed * forward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= cameraSpeed * forward;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position += left * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position -= left * cameraSpeed;
}
