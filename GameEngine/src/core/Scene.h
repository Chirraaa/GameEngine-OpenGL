#pragma once

#include "GameObject.h"
#include "../renderer/Model.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Scene
{
public:
    glm::vec3 lightPosition;
    glm::vec3 lightColor;

    Scene();

    GameObject* add(const std::string& name, Mesh* mesh, Shader* shader, Texture* texture);
    GameObject* add(const std::string& name, Model* model);
    void        draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) const;

    std::vector<std::unique_ptr<GameObject>> objects;
};