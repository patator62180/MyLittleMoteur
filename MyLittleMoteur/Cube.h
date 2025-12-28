#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Light.h"
#include "Model.h"

class Cube
{
public:
	Cube();
	void Draw(glm::vec3 position);
	void Apply(const Light& lightShader);

private:
	Shader shader;
	Model model;
};

