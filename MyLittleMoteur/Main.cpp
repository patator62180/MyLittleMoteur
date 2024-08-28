#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include "Mesh.h"
#include "Model.h"
#include "Light.h"

Camera camera;
bool firstMouse = true;
float lastX;
float lastY;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
    glm::vec3(0.7f,  0.2f,  1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f,  1.0f, 0.0f),
    glm::vec3(0.8f,  0.8f, 0.8f)
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera& camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.ProcessInput(window, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "MyLittleMoteur", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader objectShader("../shader/shader.vs", "../shader/shader.fs");
    Shader lightShader("../shader/lightShader.vs", "../shader/lightShader.fs");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::rotate(trans, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    auto lightOrigin = glm::vec3(0.0f, 0.0f, 1.0f);
    auto lightPosition = lightOrigin;
    auto lightColor = glm::vec3(1.0f);

    Model backPackModel("../model/Backpack/backpack.obj", false);
    Model cubeModel("../model/Cube/cube.obj", true);

    std::list<Light> lightList;
    for (unsigned int i = 0; i <= (unsigned int) pointLightPositions->length(); i++)
    {
        lightColor = pointLightColors[i];
        glm::vec3 diffuseColor = 0.8f * lightColor;
        glm::vec3 ambientColor = 0.2f * diffuseColor;
        glm::vec3 specularColor = glm::vec3(1.0);
        Light light(i, pointLightPositions[i], 80.0f, ambientColor, diffuseColor, specularColor);
        lightList.push_back(light);
    }

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float) glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window, camera);

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
            lightShader.setVec3("lightColor", 5.0f*light.ambient);

            cubeModel.Draw(lightShader);
            //light.Draw(cubeModel, lightShader, camera);
            light.Apply(objectShader);
        }

        //draw objects
        objectShader.use();
        objectShader.setFloat("time", (float) glfwGetTime());
        objectShader.setMat4("transform", trans);
        objectShader.setMat4("view", camera.GetLookAt());
        objectShader.setMat4("projection", camera.GetProjection());
        objectShader.setVec3("viewPosition", camera.Position.x, camera.Position.y, camera.Position.z);
        //material
        objectShader.setInt("material.diffuse", 0);
        objectShader.setInt("material.specular", 1);
        objectShader.setFloat("material.shininess", 32.0f);
        
        for (glm::vec3 position : cubePositions)
        {
            auto transform = glm::translate(glm::mat4(1.0f), position);
            transform = glm::scale(transform, glm::vec3(0.5f));
            objectShader.setMat4("model", transform);

            cubeModel.Draw(objectShader);
        }

        auto translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-7.0f));
        objectShader.setMat4("model", translate);
        backPackModel.Draw(objectShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}