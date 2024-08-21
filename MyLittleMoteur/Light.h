#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum lightType
{
	Point,
	Directionnal,
	Spot
};

class Light
{

private:
	glm::vec3 position;
	glm::quat direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float range;
	float cutoff;
	float blend;
};

