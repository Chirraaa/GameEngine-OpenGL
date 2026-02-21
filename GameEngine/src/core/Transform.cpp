#include "Transform.h"

Transform::Transform()
	: position(0.0f), rotation(0.0f), scale(1.0f)
{
}

glm::mat4 Transform::getModelMatrix() const
{
	glm::mat4 m = glm::mat4(1.0f);
	m = glm::translate(m, position);
	m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	m = glm::scale(m, scale);

	return m;
}