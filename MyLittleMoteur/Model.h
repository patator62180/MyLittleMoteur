#pragma once
#include <Shader.h>
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>

using namespace std;

class Model
{
public:
    Model(const char* path);
    void Draw(Shader& shader);
private:
    const float shininess = 32.0f;
    // model data
    vector<Mesh> meshes;
    string directory;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Material> processMaterial(aiMaterial* mat);
    unsigned int loadTextureFromFile(const char* path);
};