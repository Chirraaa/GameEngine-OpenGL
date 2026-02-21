#include "PhysicsWorld.h"
#include <glm/glm.hpp>

PhysicsWorld::PhysicsWorld(float groundY)
    : groundY(groundY)
{
}

void PhysicsWorld::add(Rigidbody* rigidbody, Transform* transform, float radius)
{
    m_bodies.push_back({ rigidbody, transform, radius });
}

void PhysicsWorld::update(float deltaTime)
{
    for (auto& body : m_bodies)
    {
        body.rigidbody->update(deltaTime);
        body.transform->position += body.rigidbody->velocity * deltaTime;
        resolveGroundCollision(body);
    }

    for (int i = 0; i < (int)m_bodies.size(); i++)
        for (int j = i + 1; j < (int)m_bodies.size(); j++)
            resolveSphereCollision(m_bodies[i], m_bodies[j]);
}

void PhysicsWorld::resolveGroundCollision(PhysicsBody& body)
{
    float bottom = body.transform->position.y - body.radius;

    if (bottom < groundY)
    {
        body.transform->position.y = groundY + body.radius;
        body.rigidbody->velocity.y = -body.rigidbody->velocity.y * body.rigidbody->restitution;

        // Damp horizontal velocity (friction)
        body.rigidbody->velocity.x *= 0.98f;
        body.rigidbody->velocity.z *= 0.98f;
    }
}

void PhysicsWorld::resolveSphereCollision(PhysicsBody& a, PhysicsBody& b)
{
    if (a.rigidbody->isStatic && b.rigidbody->isStatic) return;

    glm::vec3 diff = a.transform->position - b.transform->position;
    float     dist = glm::length(diff);
    float     minDist = a.radius + b.radius;

    if (dist >= minDist || dist < 0.0001f) return;

    glm::vec3 normal = glm::normalize(diff);
    float     overlap = minDist - dist;

    // Push objects apart based on mass
    float totalMass = a.rigidbody->mass + b.rigidbody->mass;
    if (!a.rigidbody->isStatic)
        a.transform->position += normal * overlap * (b.rigidbody->mass / totalMass);
    if (!b.rigidbody->isStatic)
        b.transform->position -= normal * overlap * (a.rigidbody->mass / totalMass);

    // Impulse response
    float e = glm::min(a.rigidbody->restitution, b.rigidbody->restitution);
    float j = -(1.0f + e) * glm::dot(a.rigidbody->velocity - b.rigidbody->velocity, normal);
    j /= (1.0f / a.rigidbody->mass) + (1.0f / b.rigidbody->mass);

    if (!a.rigidbody->isStatic) a.rigidbody->velocity += (j / a.rigidbody->mass) * normal;
    if (!b.rigidbody->isStatic) b.rigidbody->velocity -= (j / b.rigidbody->mass) * normal;
}