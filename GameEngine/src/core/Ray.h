#pragma once

#include <glm/glm.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    static Ray fromMouse(double mouseX, double mouseY,
        int width, int height,
        const glm::mat4& view,
        const glm::mat4& projection);

    bool intersectsSphere(const glm::vec3& center, float radius, float& outDist) const;
};