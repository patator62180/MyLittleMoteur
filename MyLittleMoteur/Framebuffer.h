#pragma once
#include "Shader.h"

class Framebuffer
{
public:
	Framebuffer(Shader _quadShader);
	void Setup();
	void Bind();
	void UnBind();
	void DrawDefault();

private:
	unsigned int quadVAO;
	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	Shader quadShader;
};

