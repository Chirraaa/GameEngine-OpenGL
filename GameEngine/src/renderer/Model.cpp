#include "Model.h"
#include <iostream>

Model::Model(const std::string& path, Shader* shader, Texture* texture)
    : m_shader(shader), m_texture(texture)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP: " << importer.GetErrorString() << "\n";
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::draw(const glm::mat4& view, const glm::mat4& projection,
    const glm::mat4& model,
    const glm::vec3& lightPos, const glm::vec3& lightColor,
    const glm::vec3& cameraPos) const
{
    m_shader->use();
    m_shader->setMat4("model", model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
    m_shader->setVec3("lightPos", lightPos);
    m_shader->setVec3("lightColor", lightColor);
    m_shader->setVec3("cameraPos", cameraPos);

    m_texture->bind(0);

    for (const auto& mesh : m_meshes)
        mesh->draw();
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        m_meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]]));

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

std::unique_ptr<Mesh> Model::processMesh(aiMesh* mesh)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex v;
        v.position[0] = mesh->mVertices[i].x;
        v.position[1] = mesh->mVertices[i].y;
        v.position[2] = mesh->mVertices[i].z;

        v.normal[0] = mesh->mNormals ? mesh->mNormals[i].x : 0.0f;
        v.normal[1] = mesh->mNormals ? mesh->mNormals[i].y : 0.0f;
        v.normal[2] = mesh->mNormals ? mesh->mNormals[i].z : 1.0f;

        v.texCoord[0] = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i].x : 0.0f;
        v.texCoord[1] = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i].y : 0.0f;

        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return std::make_unique<Mesh>(vertices, indices);
}