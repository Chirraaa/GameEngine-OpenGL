#pragma once

#include "Transform.h"
#include "../renderer/Mesh.h"
#include "../renderer/Shader.h"
#include "../renderer/Texture.h"
#include "../renderer/Model.h"
#include "../physics/Rigidbody.h"

#include <string>
#include <memory>
#include <glm/glm.hpp>

class GameObject
{
public:
    std::string            name;
    Transform              transform;
    std::unique_ptr<Rigidbody> rigidbody;

    GameObject(const std::string& name, Mesh* mesh, Shader* shader, Texture* texture);
    GameObject(const std::string& name, Model* model);

    void addRigidbody(float mass = 1.0f, float restitution = 0.5f, bool isStatic = false);

    void draw(const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& lightPos, const glm::vec3& lightColor,
        const glm::vec3& cameraPos) const;

private:
    Mesh* m_mesh;
    Shader* m_shader;
    Texture* m_texture;
    Model* m_model;
    bool     m_usesModel;
};