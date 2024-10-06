#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>

enum ELightType
{
	Point,
	Directionnal,
	Spot
};

class Light
{
public:
	Light(unsigned int index, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	Light(unsigned int index, glm::vec3 position, float range, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	Light(unsigned int index, glm::vec3 position, glm::vec3 direction, float range, float cutoff, float blend, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	void Apply(const Shader& shader) const;

	glm::vec3 position;
	glm::quat direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float range;
	float cutoff;
	float blend;
	ELightType type;
	unsigned int index;
};

