#include "Scene.h"

Scene::Scene()
    : lightPosition(1.2f, 1.0f, 2.0f), lightColor(1.0f)
{
}

GameObject* Scene::add(const std::string& name, Mesh* mesh, Shader* shader, Texture* texture)
{
    objects.push_back(std::make_unique<GameObject>(name, mesh, shader, texture));
    return objects.back().get();
}

GameObject* Scene::add(const std::string& name, Model* model)
{
    objects.push_back(std::make_unique<GameObject>(name, model));
    return objects.back().get();
}

void Scene::draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) const
{
    for (const auto& obj : objects)
        obj->draw(view, projection, lightPosition, lightColor, cameraPos);
}