#pragma once
#include <glm/glm.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  0.0f,  -2.0f),
};

const glm::vec3 OrthographicPositions[] = {
    glm::vec3(-5.0f,0.0f,0.0f),
    glm::vec3(-4.0f,0.0f,0.0f),
    glm::vec3(-5.0f,1.0f,0.0f),
    glm::vec3(-5.0f,0.0f,1.0f)
};

const glm::vec3 OrthographicColors[] = {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f)
};

const glm::vec3 pointLightPositions[] = {
    glm::vec3(3.0f,  2.0f,  -3.0f),
    glm::vec3(-3.0f, 2.0f, 3.0f),
    glm::vec3(3.0f,  2.0f, 3.0f),
    glm::vec3(-3.0f,  2.0f, -3.0f)
};

const glm::vec3 pointLightColors[] = {
    glm::vec3(0.7f,  0.2f,  1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f,  1.0f, 0.0f),
    glm::vec3(0.8f,  0.8f, 0.8f)
};

const glm::vec3 grassPositions[] = {
    glm::vec3(-1.5f,  0.0f, -0.48f),
    glm::vec3(1.5f,  0.0f,  0.51f),
    glm::vec3(0.0f,  0.0f,  0.7f),
    glm::vec3(-0.3f,  0.0f, -2.3f),
    glm::vec3(0.5f,  0.0f, -0.6f)
};

const float quadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};