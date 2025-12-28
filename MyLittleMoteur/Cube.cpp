#include "Cube.h"
#include "Camera.h"

Cube::Cube() : shader("../shader/shader.vs", "../shader/shader.fs"), model("../model/Cube/cube.obj", true, false) {}

void Cube::Draw(glm::vec3 position)
{
    Camera& camera = Camera::GetInstance();
    //draw objects
    shader.use();
    shader.setFloat("time", (float)glfwGetTime());
    shader.setMat4("transform", glm::mat4(1.0f));
    shader.setMat4("view", camera.GetLookAt());
    shader.setMat4("projection", camera.GetProjection());
    shader.setVec3("viewPosition", camera.Position.x, camera.Position.y, camera.Position.z);
    //material
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.setFloat("material.shininess", 32.0f);

    auto transform = glm::translate(glm::mat4(1.0f), position);
    transform = glm::scale(transform, glm::vec3(0.5f));
    shader.setMat4("model", transform);
    //lightShader.use();
    //lightShader.setMat4("model", transform);

    model.Draw(shader);
}

void Cube::Apply(const Light& light)
{
    light.Apply(shader);
}
