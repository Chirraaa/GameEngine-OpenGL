#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

struct SubMesh
{
    std::unique_ptr<Mesh> mesh;
    Texture* texture;
};

class Model
{
public:
    Model(const std::string& path, Shader* shader, Texture* fallback);

    void draw(const glm::mat4& view, const glm::mat4& projection,
        const glm::mat4& model,
        const glm::vec3& lightPos, const glm::vec3& lightColor,
        const glm::vec3& cameraPos) const;

private:
    std::vector<SubMesh>             m_subMeshes;
    std::vector<std::unique_ptr<Texture>> m_ownedTextures;
    Shader* m_shader;
    Texture* m_fallback;
    std::string m_directory;

    void     processNode(aiNode* node, const aiScene* scene);
    SubMesh  processMesh(aiMesh* mesh, const aiScene* scene);
    Texture* loadMaterialTexture(aiMaterial* mat, const aiScene* scene);
    Texture* loadEmbedded(const aiTexture* tex);
    Texture* loadFromDisk(const std::string& path);
    unsigned int createGLTexture(unsigned char* data, int w, int h, int ch);
};