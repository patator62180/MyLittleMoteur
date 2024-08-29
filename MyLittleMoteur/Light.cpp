#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Model.h"

Light::Light(unsigned int index, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->index = index;
	this->type = Directionnal;
	this->direction = direction;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

Light::Light(unsigned int index, glm::vec3 position, float range, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->index = index;
	this->type = Point;
	this->position = position;
	this->range = range;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

Light::Light(unsigned int index, glm::vec3 position, glm::vec3 direction, float range, float cutoff, float blend, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->index = index;
	this->type = Spot;
	this->position = position;
	this->direction = direction;
	this->range = range;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

void Light::Apply(const Shader& shader) const
{
    auto lightModel = glm::translate(glm::mat4(1.0), position);
	string strLight = "";
	switch (type) {
		case Directionnal:
			strLight.append("directionnalLights[");
			break;
		case Spot:
			strLight.append("spotLights[");
			break;
		case Point:
			strLight.append("pointLights[");
			break;
	}

    strLight.append(std::to_string(index) + "]");
    shader.use();
	shader.setBool(strLight + ".enabled", true);
	shader.setVec3(strLight + ".ambient", ambient);
	shader.setVec3(strLight + ".diffuse", diffuse);
	shader.setVec3(strLight + ".specular", specular);

	shader.setVec3(strLight + ".position", position);
	shader.setVec3(strLight + ".direction", glm::eulerAngles(direction));
	shader.setFloat(strLight + ".attenuationLinear", 4.5f / range);
	shader.setFloat(strLight + ".attenuationQuad", 75.0f / (range * range));
	shader.setFloat(strLight + ".cutoff", cutoff);
	shader.setFloat(strLight + ".blend", blend);
}