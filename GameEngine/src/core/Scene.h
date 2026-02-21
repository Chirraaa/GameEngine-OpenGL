#pragma once

#include "GameObject.h"
#include <vector>
#include <memory>

class Scene
{
public:
	std::vector<std::unique_ptr<GameObject>> objects;

	GameObject* add(const std::string& name, Mesh* mesh, Shader* shader, Texture* texture);
	
	void draw(const glm::mat4& view, const glm::mat4& projection) const;
};