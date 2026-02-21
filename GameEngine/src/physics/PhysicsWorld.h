#pragma once

#include "Rigidbody.h"
#include "../core/Transform.h"
#include <vector>

struct PhysicsBody
{
    Rigidbody* rigidbody;
    Transform* transform;
    float      radius;
};

class PhysicsWorld
{
public:
    float groundY;

    PhysicsWorld(float groundY = 0.0f);

    void add(Rigidbody* rigidbody, Transform* transform, float radius);
    void update(float deltaTime);

private:
    std::vector<PhysicsBody> m_bodies;

    void resolveGroundCollision(PhysicsBody& body);
    void resolveSphereCollision(PhysicsBody& a, PhysicsBody& b);
};