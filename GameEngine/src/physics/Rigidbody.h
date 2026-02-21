#pragma once

#include <glm/glm.hpp>

class Rigidbody
{
public:
    float     mass;
    float     restitution;
    bool      isStatic;

    glm::vec3 velocity;
    glm::vec3 forces;

    Rigidbody(float mass = 1.0f, float restitution = 0.5f, bool isStatic = false);

    void applyForce(const glm::vec3& force);
    void update(float deltaTime);
    void reset();
};