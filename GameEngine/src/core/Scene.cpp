#include "Scene.h"

GameObject* Scene::add(const std::string& name, Mesh* mesh, Shader* shader, Texture* texture) {
	objects.push_back(std::make_unique<GameObject>(name, mesh, shader, texture));

	return objects.back().get();
}

void Scene::draw(const glm::mat4& view, const glm::mat4& projection) const {
	for (const auto& obj : objects)
		obj->draw(view, projection);
}