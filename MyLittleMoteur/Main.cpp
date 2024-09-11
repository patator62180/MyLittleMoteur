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
    glm::vec3(2.0f,  0.0f,  -2.0f),
};

glm::vec3 OrthographicPositions[] = {
    glm::vec3(-5.0f,0.0f,0.0f),
    glm::vec3(-4.0f,0.0f,0.0f),
    glm::vec3(-5.0f,1.0f,0.0f),
    glm::vec3(-5.0f,0.0f,1.0f)
};

glm::vec3 OrthographicColors[] = {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f)
};

glm::vec3 pointLightPositions[] = {
    glm::vec3(3.0f,  2.0f,  -3.0f),
    glm::vec3(-3.0f, 2.0f, 3.0f),
    glm::vec3(3.0f,  2.0f, 3.0f),
    glm::vec3(-3.0f,  2.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
    glm::vec3(0.7f,  0.2f,  1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f,  1.0f, 0.0f),
    glm::vec3(0.8f,  0.8f, 0.8f)
};

glm::vec3 grassPositions[] = {
    glm::vec3(-1.5f,  0.0f, -0.48f),
    glm::vec3(1.5f,  0.0f,  0.51f),
    glm::vec3(0.0f,  0.0f,  0.7f),
    glm::vec3(-0.3f,  0.0f, -2.3f),
    glm::vec3(0.5f,  0.0f, -0.6f)
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

void draw_OrthonormalReference(const Model& cubeModel, const Shader& shader) 
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
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

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

    //Model backPackModel("../model/Backpack/backpack.obj", false);
    Model cubeModel("../model/Cube/cube.obj", true, false);
    Model planeModel("../model/Plane/plane.obj", false, false);
    Model grassModel("../model/Grass/grass.obj", true, true);

    std::list<Light> lightList;
    for (unsigned int i = 0; i <= (unsigned int) pointLightPositions->length(); i++)
    {
        lightColor = pointLightColors[i];
        glm::vec3 diffuseColor = 1.0f * lightColor;
        glm::vec3 ambientColor = 0.5f * diffuseColor;
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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

			cubeModel.DrawWithOutline(objectShader, lightShader);
		}


		transform = glm::scale(glm::mat4(1.0), glm::vec3(2.0f));
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -2.0f));
        objectShader.use();
		objectShader.setMat4("model", transform);
		//backPackModel.Draw(objectShader);

        for (auto position : grassPositions) 
        {
            objectShader.use();
            auto model = glm::translate(glm::mat4(1.0), position);
            model = glm::rotate(model, -glm::half_pi<float>(), glm::vec3(1.0, 0.0, 0.0));
            model = glm::scale(model, glm::vec3(0.5));
            objectShader.setMat4("model", model);

            grassModel.Draw(objectShader);
        }

        draw_OrthonormalReference(cubeModel, lightShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}