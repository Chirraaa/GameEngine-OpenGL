#include "GameObject.h"

GameObject::GameObject(const std::string& name, Mesh* mesh, Shader* shader, Texture* texture)
	: name(name), m_mesh(mesh), m_shader(shader), m_texture(texture)
{ }

void GameObject::draw(const glm::mat4& view, const glm::mat4& projection) const
{
	m_shader->use();
	m_shader->setMat4("model", transform.getModelMatrix());
	m_shader->setMat4("view", view);
	m_shader->setMat4("projection", projection);

	m_texture->bind(0);
	m_mesh->draw();
}