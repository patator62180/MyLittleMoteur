#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <stb/stb_image.h>
#include <cpplocate/cpplocate.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Light.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera;
bool firstMouse = true;
float lastX;
float lastY;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 0.0f, -2.0f),
};

glm::vec3 OrthographicPositions[] = {
    glm::vec3(-5.0f, 0.0f, 0.0f),
    glm::vec3(-4.0f, 0.0f, 0.0f),
    glm::vec3(-5.0f, 1.0f, 0.0f),
    glm::vec3(-5.0f, 0.0f, 1.0f)};

glm::vec3 OrthographicColors[] = {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f)};

glm::vec3 pointLightPositions[] = {
    glm::vec3(3.0f, 2.0f, -3.0f),
    glm::vec3(-3.0f, 2.0f, 3.0f),
    glm::vec3(3.0f, 2.0f, 3.0f),
    glm::vec3(-3.0f, 2.0f, -3.0f)};

glm::vec3 pointLightColors[] = {
    glm::vec3(0.7f, 0.2f, 1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.8f, 0.8f, 0.8f)};

glm::vec3 grassPositions[] = {
    glm::vec3(-1.5f, 0.0f, -0.48f),
    glm::vec3(1.5f, 0.0f, 0.51f),
    glm::vec3(0.0f, 0.0f, 0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3(0.5f, 0.0f, -0.6f)};

float quadVertices[] = {
    // positions   // texCoords
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f};

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Camera &camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.ProcessInput(window, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
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

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void draw_OrthonormalReference(const Model &cubeModel, const Shader &shader)
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

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyLittleMoteur", NULL, NULL);
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

    Shader objectShader("res/shader/shader.vs", "res/shader/shader.fs");
    Shader lightShader("res/shader/lightShader.vs", "res/shader/lightShader.fs");
    Shader quadShader("res/shader/quadShader.vs", "res/shader/quadShader.fs");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);

    glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::rotate(trans, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), SCR_HEIGHT / (float)SCR_WIDTH, 0.1f, 100.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    auto lightOrigin = glm::vec3(0.0f, 0.0f, 1.0f);
    auto lightPosition = lightOrigin;
    auto lightColor = glm::vec3(1.0f);

    // Model backPackModel("res/model/Backpack/backpack.obj", false);
    Model cubeModel("res/model/Cube/cube.obj", true, false);
    Model planeModel("res/model/Plane/plane.obj", false, false);
    Model grassModel("res/model/Glass/glass.obj", true, true);

    std::list<Light> lightList;
    for (unsigned int i = 0; i <= (unsigned int)pointLightPositions->length(); i++)
    {
        lightColor = pointLightColors[i];
        glm::vec3 diffuseColor = 0.5f * lightColor;
        glm::vec3 ambientColor = 0.3f * diffuseColor;
        glm::vec3 specularColor = glm::vec3(1.0);
        Light light(i, pointLightPositions[i], 80.0f, ambientColor, diffuseColor, specularColor);
        lightList.push_back(light);
    }

    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    // framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);           // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window, camera);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Light &light : lightList)
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
        }

        // draw objects
        objectShader.use();
        objectShader.setFloat("time", (float)glfwGetTime());
        objectShader.setMat4("transform", trans);
        objectShader.setMat4("view", camera.GetLookAt());
        objectShader.setMat4("projection", camera.GetProjection());
        objectShader.setVec3("viewPosition", camera.Position.x, camera.Position.y, camera.Position.z);
        // material
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
            auto transform = glm::translate(glm::mat4(1.0f), position);
            transform = glm::scale(transform, glm::vec3(0.5f));
            objectShader.use();
            objectShader.setMat4("model", transform);
            transform = glm::scale(transform, glm::vec3(1.05f));
            lightShader.use();
            lightShader.setMat4("model", transform);

            cubeModel.Draw(objectShader);
        }

        transform = glm::scale(glm::mat4(1.0), glm::vec3(2.0f));
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -2.0f));
        objectShader.use();
        objectShader.setMat4("model", transform);
        // backPackModel.Draw(objectShader);

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

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        draw_OrthonormalReference(cubeModel, lightShader);

        quadShader.use();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        quadShader.use();
        quadShader.setInt("screenTexture", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}