#pragma once
#include <Shader.h>
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>

using namespace std;

class Model
{
public:
    Model(const char* path, bool handleAlpha, bool handleAlphaInternal);
    void Draw(const Shader& shader) const;
    void DrawWithOutline(const Shader& shader, const Shader& outlineShader) const;
private:
    // model data
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;
    bool handleAlpha;
    bool handleAlphaInternal;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    Texture createTexture(const char* fileName, std::string directory, std::string type);
};