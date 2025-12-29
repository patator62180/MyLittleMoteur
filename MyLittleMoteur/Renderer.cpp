#include "Renderer.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Model.h"
#include "Light.h"
#include "Constants.h"
#include <map>

Renderer::Renderer():
    objectShader("../shader/shader.vs", "../shader/shader.fs"),
    lightShader("../shader/lightShader.vs", "../shader/lightShader.fs"),
    quadShader("../shader/quadShader.vs", "../shader/quadShader.fs"),

    //Model backPackModel("../model/Backpack/backpack.obj", false);
    cubeModel("../model/Cube/cube.obj", true, false),
    planeModel("../model/Plane/plane.obj", false, false),
    grassModel("../model/Glass/glass.obj", true, true),
    camera(Camera::GetInstance()),
    framebuffer(quadShader)
{
}

void Renderer::Setup()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);

    glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::rotate(trans, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), SCR_HEIGHT / (float)SCR_WIDTH, 0.1f, 100.0f);

    auto lightOrigin = glm::vec3(0.0f, 0.0f, 1.0f);
    auto lightPosition = lightOrigin;
    auto lightColor = glm::vec3(1.0f);

    for (unsigned int i = 0; i <= (unsigned int)pointLightPositions->length(); i++)
    {
        lightColor = pointLightColors[i];
        glm::vec3 diffuseColor = 0.5f * lightColor;
        glm::vec3 ambientColor = 0.3f * diffuseColor;
        glm::vec3 specularColor = glm::vec3(1.0);
        Light light(i, pointLightPositions[i], 80.0f, ambientColor, diffuseColor, specularColor);
        lightList.push_back(light);
    }

    framebuffer.Setup();
}

void Renderer::Draw()
{
    framebuffer.Bind();
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Light& light : lightList)
    {
        auto lightModel = glm::translate(glm::mat4(1.0), light.position);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));

        lightShader.use();
        lightShader.setMat4("transform", glm::mat4(1.0f));
        lightShader.setMat4("model", lightModel);
        lightShader.setMat4("view", camera.GetLookAt());
        lightShader.setMat4("projection", camera.GetProjection());
        lightShader.setVec3("lightColor", 5.0f * light.ambient);

        cubeModel.Draw(lightShader);
        light.Apply(objectShader);
        cube.Apply(light);
    }

    //draw objects
    objectShader.use();
    objectShader.setFloat("time", (float)glfwGetTime());
    objectShader.setMat4("transform", trans);
    objectShader.setMat4("view", camera.GetLookAt());
    objectShader.setMat4("projection", camera.GetProjection());
    objectShader.setVec3("viewPosition", camera.Position.x, camera.Position.y, camera.Position.z);
    //material
    objectShader.setInt("material.diffuse", 0);
    objectShader.setInt("material.specular", 1);
    objectShader.setFloat("material.shininess", 32.0f);


    glStencilMask(0x00);
    auto transform = glm::scale(glm::mat4(1.0), glm::vec3(3.0f));
    transform = glm::translate(transform, glm::vec3(0.0f, -0.171f, 0.0f));
    objectShader.setMat4("model", transform);
    planeModel.Draw(objectShader);

    for (glm::vec3 position : cubePositions)
    {
        cube.Draw(position);
    }


    transform = glm::scale(glm::mat4(1.0), glm::vec3(2.0f));
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -2.0f));
    objectShader.use();
    objectShader.setMat4("model", transform);
    //backPackModel.Draw(objectShader);

    std::map<float, glm::vec3> sorted;
    for (glm::vec3 position : grassPositions)
    {
        float distance = glm::length(camera.Position - position);
        sorted[distance] = position;
    }

    for (auto it = sorted.rbegin(); it != sorted.rend(); it++)
    {
        objectShader.use();
        auto model = glm::translate(glm::mat4(1.0), it->second);
        model = glm::rotate(model, glm::half_pi<float>(), glm::vec3(1.0, 0.0, 0.0));
        model = glm::scale(model, glm::vec3(0.5));
        objectShader.setMat4("model", model);

        grassModel.Draw(objectShader);
    }
    draw_OrthonormalReference(cubeModel, lightShader);

    framebuffer.DrawDefault();
}

void Renderer::draw_OrthonormalReference(const Model& cubeModel, const Shader& shader)
{
    for (unsigned int i = 0; i <= (unsigned int)OrthographicPositions->length(); i++)
    {
        auto position = OrthographicPositions[i];
        auto color = OrthographicColors[i];
        auto transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::scale(transform, glm::vec3(0.25f));
        shader.use();
        shader.setMat4("model", transform);
        shader.setVec3("lightColor", color);

        cubeModel.Draw(shader);
    }
}

