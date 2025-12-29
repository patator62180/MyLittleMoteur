#pragma once
#include "Shader.h"
#include "Model.h"
#include "Cube.h"
#include "Light.h"
#include "Camera.h"

class Renderer
{
public:
	void Setup();
	void Draw();
	Renderer();
private:
	void draw_OrthonormalReference(const Model& cubeModel, const Shader& shader);

	unsigned int framebuffer;
	Shader objectShader;
	Shader lightShader;
	Shader quadShader;

	//Model backPackModel;
	Model cubeModel;
	Model planeModel;
	Model grassModel;

	std::list<Light> lightList;
	Camera& camera;

	Cube cube;

	glm::mat4 trans = glm::mat4(1.0f);

	unsigned int quadVAO;

	unsigned int textureColorbuffer;
};

