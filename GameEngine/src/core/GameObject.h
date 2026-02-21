#pragma once

#include "Transform.h"
#include "../renderer/Mesh.h"
#include "../renderer/Shader.h"
#include "../renderer/Texture.h"

#include <string>

class GameObject
{
public:
	std::string name;
	Transform transform;

	GameObject(const std::string& name, Mesh* mesh, Shader* shader, Texture* texture);

	void draw(const glm::mat4& view, const glm::mat4& projection) const;

private:
	Mesh* m_mesh;
	Shader* m_shader;
	Texture* m_texture;
};