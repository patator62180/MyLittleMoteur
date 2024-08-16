#pragma once
#include <glm/fwd.hpp>
#include <glm/ext/vector_float3.hpp>
#include <string>
#include <vector>
#include <Shader.h>


using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Material {
	unsigned int diffuseId;
	unsigned int specularId;
    float shininess;
};

class Mesh {
public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Material> materials;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Material> materials);
    void Draw(Shader& shader);
private:
    const int nbMaterials = 3;
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};