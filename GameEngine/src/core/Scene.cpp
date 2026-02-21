#include "Scene.h"
#include <glm/glm.hpp>

Scene::Scene()
    : lightPosition(5.0f, 10.0f, 5.0f), lightColor(1.0f), selectedIndex(-1)
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

void Scene::remove(int index)
{
    if (index < 0 || index >= (int)objects.size()) return;
    objects.erase(objects.begin() + index);
    selectedIndex = -1;
}

int Scene::raycast(const Ray& ray) const
{
    int   closest = -1;
    float closestDist = FLT_MAX;

    for (int i = 0; i < (int)objects.size(); i++)
    {
        float radius = glm::length(objects[i]->transform.scale) * 0.8f;
        float dist = 0.0f;
        if (ray.intersectsSphere(objects[i]->transform.position, radius, dist))
        {
            if (dist < closestDist)
            {
                closestDist = dist;
                closest = i;
            }
        }
    }

    return closest;
}

void Scene::drawDepthPass(Shader& depthShader) const
{
    depthShader.use();
    for (const auto& obj : objects)
        obj->drawDepth(depthShader);
}

void Scene::draw(const glm::mat4& view, const glm::mat4& projection,
    const glm::vec3& cameraPos,
    const glm::mat4& lightSpaceMatrix,
    unsigned int shadowMapSlot) const
{
    for (const auto& obj : objects)
        obj->draw(view, projection, lightPosition, lightColor,
            cameraPos, lightSpaceMatrix, shadowMapSlot);
}