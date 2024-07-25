#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>

Camera::Camera()
{
	Position = glm::vec3(0.0, 0.0, 3.0);
	pitch = 0;
	yaw = 0;
	roll = 0;
	forward = glm::vec3(0.0, 0.0, -1.0);
	up = glm::vec3(0.0, 1.0, 0.0);
	left = glm::vec3(-1.0, 0.0, 0.0);
	zoom = 45;
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

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(glm::radians(zoom), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Camera::UpdateValues()
{
	glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0), glm::radians(-yaw), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0), glm::radians(pitch), glm::vec3(-1.0, 0.0, 0.0));
	forward = yawRotation * pitchRotation * glm::vec4(0.0, 0.0, -1.0, 1.0);
	left = -glm::cross(forward, world_up);
	left = glm::normalize(left);
	up = glm::cross(forward, left);
}

void Camera::DebugVec3(glm::vec3 vector) 
{
	std::cout << vector.x << " ; " << vector.y << " ; " << vector.z << std::endl;
}


void Camera::ProcessInput(GLFWwindow* window, float deltaTime)
{
	auto factor = deltaTime * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += factor * forward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= factor * forward;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position += factor * left;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position -= factor * left;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	pitch = glm::max(-89.0f, glm::min(pitch, 89.0f));

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateValues();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	zoom -= (float)yoffset;
	zoom = glm::max(minZoom, glm::min(zoom, maxZoom));
	UpdateValues();
}
