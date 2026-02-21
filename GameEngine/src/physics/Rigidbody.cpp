#include "Rigidbody.h"

static const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.81f, 0.0f);

Rigidbody::Rigidbody(float mass, float restitution, bool isStatic)
    : mass(mass), restitution(restitution), isStatic(isStatic),
    velocity(0.0f), forces(0.0f)
{
}

void Rigidbody::applyForce(const glm::vec3& force)
{
    if (!isStatic)
        forces += force;
}

void Rigidbody::update(float deltaTime)
{
    if (isStatic) return;

    glm::vec3 acceleration = GRAVITY + (forces / mass);
    velocity += acceleration * deltaTime;
    forces = glm::vec3(0.0f);
}

void Rigidbody::reset()
{
    velocity = glm::vec3(0.0f);
    forces = glm::vec3(0.0f);
}