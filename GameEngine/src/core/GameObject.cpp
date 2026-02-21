#include "GameObject.h"

GameObject::GameObject(const std::string& name, Mesh* mesh, Shader* shader, Texture* texture)
    : name(name), m_mesh(mesh), m_shader(shader), m_texture(texture), m_model(nullptr), m_usesModel(false)
{
}

GameObject::GameObject(const std::string& name, Model* model)
    : name(name), m_mesh(nullptr), m_shader(nullptr), m_texture(nullptr), m_model(model), m_usesModel(true)
{
}

void GameObject::addRigidbody(float mass, float restitution, bool isStatic)
{
    rigidbody = std::make_unique<Rigidbody>(mass, restitution, isStatic);
}

void GameObject::draw(const glm::mat4& view, const glm::mat4& projection,
    const glm::vec3& lightPos, const glm::vec3& lightColor,
    const glm::vec3& cameraPos) const
{
    if (m_usesModel)
    {
        m_model->draw(view, projection, transform.getModelMatrix(), lightPos, lightColor, cameraPos);
        return;
    }

    m_shader->use();
    m_shader->setMat4("model", transform.getModelMatrix());
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
    m_shader->setVec3("lightPos", lightPos);
    m_shader->setVec3("lightColor", lightColor);
    m_shader->setVec3("cameraPos", cameraPos);

    m_texture->bind(0);
    m_mesh->draw();
}