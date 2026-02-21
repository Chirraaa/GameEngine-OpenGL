#pragma once

#include "Mesh.h"
#include <memory>

class Primitives
{
public:
    static std::unique_ptr<Mesh> cube();
    static std::unique_ptr<Mesh> sphere(int stacks = 16, int slices = 16);
    static std::unique_ptr<Mesh> tetrahedron();
    static std::unique_ptr<Mesh> capsule(float radius = 0.5f, float height = 1.0f, int stacks = 8, int slices = 16);
    static std::unique_ptr<Mesh> plane(float size = 1.0f);
    static std::unique_ptr<Mesh> circle(float radius = 0.5f, int slices = 32);
};