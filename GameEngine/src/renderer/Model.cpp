#include "Model.h"
#include "stb/stb_image.h"
#include <iostream>

Model::Model(const std::string& path, Shader* shader, Texture* fallback)
    : m_shader(shader), m_fallback(fallback)
{
    m_directory = path.substr(0, path.find_last_of("/\\"));

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

    for (const auto& sub : m_subMeshes)
    {
        sub.texture->bind(0);
        sub.mesh->draw();
    }
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        m_subMeshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

SubMesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
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

    SubMesh sub;
    sub.mesh = std::make_unique<Mesh>(vertices, indices);

    Texture* tex = nullptr;
    if (mesh->mMaterialIndex >= 0)
        tex = loadMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], scene);

    sub.texture = tex ? tex : m_fallback;
    return sub;
}

Texture* Model::loadMaterialTexture(aiMaterial* mat, const aiScene* scene)
{
    for (int t = 0; t <= 21; t++)
    {
        aiString texPath;
        if (mat->GetTexture((aiTextureType)t, 0, &texPath) == AI_SUCCESS)
            std::cout << "  Type " << t << " -> " << texPath.C_Str() << "\n";
    }

    aiTextureType types[] = { aiTextureType_BASE_COLOR, aiTextureType_DIFFUSE };
    for (aiTextureType type : types)
    {
        aiString texPath;
        if (mat->GetTexture(type, 0, &texPath) != AI_SUCCESS)
            continue;

        if (texPath.C_Str()[0] == '*')
        {
            int index = atoi(texPath.C_Str() + 1);
            if (index >= 0 && index < (int)scene->mNumTextures)
                return loadEmbedded(scene->mTextures[index]);
            continue;
        }

        return loadFromDisk(m_directory + "/" + texPath.C_Str());
    }

    return nullptr;
}

Texture* Model::loadEmbedded(const aiTexture* tex)
{
    unsigned char* data = nullptr;
    int w, h, ch;

    if (tex->mHeight == 0)
    {
        stbi_set_flip_vertically_on_load(false);
        data = stbi_load_from_memory(
            reinterpret_cast<unsigned char*>(tex->pcData),
            (int)tex->mWidth, &w, &h, &ch, 4);
        ch = 4;
    }
    else
    {
        w = (int)tex->mWidth;
        h = (int)tex->mHeight;
        ch = 4;
        int pixelCount = w * h;
        data = new unsigned char[pixelCount * 4];
        for (int i = 0; i < pixelCount; i++)
        {
            data[i * 4 + 0] = tex->pcData[i].r;
            data[i * 4 + 1] = tex->pcData[i].g;
            data[i * 4 + 2] = tex->pcData[i].b;
            data[i * 4 + 3] = tex->pcData[i].a;
        }
    }

    if (!data)
    {
        std::cerr << "ERROR::TEXTURE::EMBEDDED_DECODE_FAILED\n";
        return nullptr;
    }

    unsigned int id = createGLTexture(data, w, h, ch);

    if (tex->mHeight == 0)
        stbi_image_free(data);
    else
        delete[] data;

    auto t = std::make_unique<Texture>();
    t->ID = id;
    Texture* ptr = t.get();
    m_ownedTextures.push_back(std::move(t));
    return ptr;
}

Texture* Model::loadFromDisk(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);
    int w, h, ch;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 4);
    ch = 4;

    if (!data)
    {
        std::cerr << "ERROR::TEXTURE::FILE_NOT_FOUND: " << path << "\n";
        return nullptr;
    }

    unsigned int id = createGLTexture(data, w, h, ch);
    stbi_image_free(data);

    auto t = std::make_unique<Texture>();
    t->ID = id;
    Texture* ptr = t.get();
    m_ownedTextures.push_back(std::move(t));
    return ptr;
}

unsigned int Model::createGLTexture(unsigned char* data, int w, int h, int ch)
{
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum fmt = (ch == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return id;
}