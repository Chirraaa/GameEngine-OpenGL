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

class Model
{
public:
    Model(const std::string& path, Shader* shader, Texture* texture);

    void draw(const glm::mat4& view, const glm::mat4& projection,
        const glm::mat4& model,
        const glm::vec3& lightPos, const glm::vec3& lightColor,
        const glm::vec3& cameraPos) const;

private:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    Shader* m_shader;
    Texture* m_texture;

    void processNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> processMesh(aiMesh* mesh);
};