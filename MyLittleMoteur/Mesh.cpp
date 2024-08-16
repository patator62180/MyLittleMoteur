#include "Mesh.h"
#include <vector>

using namespace std;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Material> materials)
{
	this->vertices = vertices;
	this->indices = indices;
	this->materials = materials;

	setupMesh();
}

void Mesh::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < materials.size(); i++)
    {
        Material material = materials[i];
        string iStr = std::to_string(i);

        shader.setInt(("materials[" + iStr + "].diffuse"), 2*i);
        glActiveTexture(GL_TEXTURE0 + 2 * i);
        glBindTexture(GL_TEXTURE_2D, material.diffuseId);

        shader.setInt(("materials[" + iStr + "].specular"), 2*i+1);
        glActiveTexture(GL_TEXTURE0 + 2 * i + 1);
        glBindTexture(GL_TEXTURE_2D, material.specularId);

        shader.setFloat(("materials[" + iStr + "].shininess"), material.shininess);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    //texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}