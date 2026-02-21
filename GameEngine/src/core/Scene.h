#pragma once

#include "GameObject.h"
#include "../renderer/Model.h"
#include "../renderer/ShadowMap.h"
#include "Ray.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Scene
{
public:
    glm::vec3 lightPosition;
    glm::vec3 lightColor;
    int       selectedIndex;

    Scene();

    GameObject* add(const std::string& name, Mesh* mesh, Shader* shader, Texture* texture);
    GameObject* add(const std::string& name, Model* model);
    void        remove(int index);

    int  raycast(const Ray& ray) const;

    void drawDepthPass(Shader& depthShader) const;
    void draw(const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& cameraPos,
        const glm::mat4& lightSpaceMatrix,
        unsigned int shadowMapSlot) const;

    std::vector<std::unique_ptr<GameObject>> objects;
};