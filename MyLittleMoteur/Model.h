#pragma once
#include <Shader.h>
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>

using namespace std;

class Model
{
public:
    Model(const char* path, bool handleAlpha);
    void Draw(Shader& shader);
private:
    // model data
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;
    bool handleAlpha;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    Texture createTexture(const char* fileName, std::string directory, std::string type);
};