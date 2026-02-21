#include "Ray.h"
#include <glm/gtc/matrix_transform.hpp>

Ray Ray::fromMouse(double mouseX, double mouseY,
    int width, int height,
    const glm::mat4& view,
    const glm::mat4& projection)
{
    float ndcX = (2.0f * (float)mouseX / width) - 1.0f;
    float ndcY = -(2.0f * (float)mouseY / height) + 1.0f;

    glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    glm::vec4 eyeCoords = glm::inverse(projection) * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
    glm::vec4 worldCoords = glm::inverse(view) * eyeCoords;

    Ray ray;
    ray.origin = glm::vec3(glm::inverse(view)[3]);
    ray.direction = glm::normalize(glm::vec3(worldCoords));
    return ray;
}

bool Ray::intersectsSphere(const glm::vec3& center, float radius, float& outDist) const
{
    glm::vec3 oc = origin - center;
    float a = glm::dot(direction, direction);
    float b = 2.0f * glm::dot(oc, direction);
    float c = glm::dot(oc, oc) - radius * radius;
    float d = b * b - 4 * a * c;

    if (d < 0) return false;

    outDist = (-b - sqrt(d)) / (2.0f * a);
    return outDist > 0.0f;
}