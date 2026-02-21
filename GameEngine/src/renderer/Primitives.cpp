#include "Primitives.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

static Vertex makeVertex(float x, float y, float z, float nx, float ny, float nz, float u = 0, float v = 0)
{
    Vertex vert;
    vert.position[0] = x;  vert.position[1] = y;  vert.position[2] = z;
    vert.normal[0] = nx; vert.normal[1] = ny; vert.normal[2] = nz;
    vert.texCoord[0] = u;  vert.texCoord[1] = v;
    return vert;
}

std::unique_ptr<Mesh> Primitives::cube()
{
    std::vector<Vertex> vertices = {
        makeVertex(-0.5f,-0.5f,-0.5f, 0, 0,-1), makeVertex(0.5f,-0.5f,-0.5f, 0, 0,-1), makeVertex(0.5f, 0.5f,-0.5f, 0, 0,-1), makeVertex(-0.5f, 0.5f,-0.5f, 0, 0,-1),
        makeVertex(-0.5f,-0.5f, 0.5f, 0, 0, 1), makeVertex(0.5f,-0.5f, 0.5f, 0, 0, 1), makeVertex(0.5f, 0.5f, 0.5f, 0, 0, 1), makeVertex(-0.5f, 0.5f, 0.5f, 0, 0, 1),
        makeVertex(-0.5f,-0.5f,-0.5f,-1, 0, 0), makeVertex(-0.5f, 0.5f,-0.5f,-1, 0, 0), makeVertex(-0.5f, 0.5f, 0.5f,-1, 0, 0), makeVertex(-0.5f,-0.5f, 0.5f,-1, 0, 0),
        makeVertex(0.5f,-0.5f,-0.5f, 1, 0, 0), makeVertex(0.5f, 0.5f,-0.5f, 1, 0, 0), makeVertex(0.5f, 0.5f, 0.5f, 1, 0, 0), makeVertex(0.5f,-0.5f, 0.5f, 1, 0, 0),
        makeVertex(-0.5f,-0.5f,-0.5f, 0,-1, 0), makeVertex(0.5f,-0.5f,-0.5f, 0,-1, 0), makeVertex(0.5f,-0.5f, 0.5f, 0,-1, 0), makeVertex(-0.5f,-0.5f, 0.5f, 0,-1, 0),
        makeVertex(-0.5f, 0.5f,-0.5f, 0, 1, 0), makeVertex(0.5f, 0.5f,-0.5f, 0, 1, 0), makeVertex(0.5f, 0.5f, 0.5f, 0, 1, 0), makeVertex(-0.5f, 0.5f, 0.5f, 0, 1, 0),
    };

    std::vector<unsigned int> indices;
    for (int f = 0; f < 6; f++)
    {
        unsigned int b = f * 4;
        indices.insert(indices.end(), { b, b + 1, b + 2, b, b + 2, b + 3 });
    }

    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> Primitives::sphere(int stacks, int slices)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= stacks; i++)
    {
        float phi = glm::pi<float>() * i / stacks;
        for (int j = 0; j <= slices; j++)
        {
            float theta = 2.0f * glm::pi<float>() * j / slices;
            float x = sin(phi) * cos(theta);
            float y = cos(phi);
            float z = sin(phi) * sin(theta);
            vertices.push_back(makeVertex(x * 0.5f, y * 0.5f, z * 0.5f, x, y, z));
        }
    }

    for (int i = 0; i < stacks; i++)
        for (int j = 0; j < slices; j++)
        {
            unsigned int a = i * (slices + 1) + j;
            unsigned int b = a + slices + 1;
            indices.insert(indices.end(), { a, b, a + 1, b, b + 1, a + 1 });
        }

    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> Primitives::tetrahedron()
{
    float s = 0.5f;
    glm::vec3 p0(0, s, 0);
    glm::vec3 p1(-s, -s, s);
    glm::vec3 p2(s, -s, s);
    glm::vec3 p3(0, -s, -s);

    auto faceNormal = [](glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        return glm::normalize(glm::cross(b - a, c - a));
        };

    std::vector<Vertex> vertices;
    auto addFace = [&](glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        glm::vec3 n = faceNormal(a, b, c);
        vertices.push_back(makeVertex(a.x, a.y, a.z, n.x, n.y, n.z));
        vertices.push_back(makeVertex(b.x, b.y, b.z, n.x, n.y, n.z));
        vertices.push_back(makeVertex(c.x, c.y, c.z, n.x, n.y, n.z));
        };

    addFace(p0, p1, p2);
    addFace(p0, p2, p3);
    addFace(p0, p3, p1);
    addFace(p1, p3, p2);

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < 12; i++) indices.push_back(i);

    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> Primitives::capsule(float radius, float height, int stacks, int slices)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    float halfH = height * 0.5f;

    // Top hemisphere
    for (int i = 0; i <= stacks; i++)
    {
        float phi = glm::half_pi<float>() * i / stacks;
        for (int j = 0; j <= slices; j++)
        {
            float theta = 2.0f * glm::pi<float>() * j / slices;
            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);
            vertices.push_back(makeVertex(x * radius, y * radius + halfH, z * radius, x, y, z));
        }
    }

    int topCount = (int)vertices.size();

    // Bottom hemisphere
    for (int i = 0; i <= stacks; i++)
    {
        float phi = glm::half_pi<float>() + glm::half_pi<float>() * i / stacks;
        for (int j = 0; j <= slices; j++)
        {
            float theta = 2.0f * glm::pi<float>() * j / slices;
            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);
            vertices.push_back(makeVertex(x * radius, y * radius - halfH, z * radius, x, y, z));
        }
    }

    // Cylinder middle
    int cylBase = (int)vertices.size();
    for (int j = 0; j <= slices; j++)
    {
        float theta = 2.0f * glm::pi<float>() * j / slices;
        float x = cos(theta);
        float z = sin(theta);
        vertices.push_back(makeVertex(x * radius, halfH, z * radius, x, 0, z));
        vertices.push_back(makeVertex(x * radius, -halfH, z * radius, x, 0, z));
    }

    // Hemisphere indices
    auto addHemi = [&](int base, int count)
        {
            for (int i = 0; i < stacks; i++)
                for (int j = 0; j < slices; j++)
                {
                    unsigned int a = base + i * (slices + 1) + j;
                    unsigned int b = a + slices + 1;
                    indices.insert(indices.end(), { a, b, a + 1, b, b + 1, a + 1 });
                }
        };

    addHemi(0, topCount);
    addHemi(topCount, (int)vertices.size() - topCount - (slices + 1) * 2);

    // Cylinder indices
    for (int j = 0; j < slices; j++)
    {
        unsigned int a = cylBase + j * 2;
        indices.insert(indices.end(), { a, a + 1, a + 2, a + 1, a + 3, a + 2 });
    }

    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> Primitives::plane(float size)
{
    float h = size * 0.5f;
    std::vector<Vertex> vertices = {
        makeVertex(-h, 0, -h, 0, 1, 0),
        makeVertex(h, 0, -h, 0, 1, 0),
        makeVertex(h, 0,  h, 0, 1, 0),
        makeVertex(-h, 0,  h, 0, 1, 0),
    };
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };
    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> Primitives::circle(float radius, int slices)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(makeVertex(0, 0, 0, 0, 1, 0));

    for (int i = 0; i <= slices; i++)
    {
        float theta = 2.0f * glm::pi<float>() * i / slices;
        float x = cos(theta) * radius;
        float z = sin(theta) * radius;
        vertices.push_back(makeVertex(x, 0, z, 0, 1, 0));
    }

    for (int i = 1; i <= slices; i++)
        indices.insert(indices.end(), { 0, (unsigned int)i, (unsigned int)(i + 1) });

    return std::make_unique<Mesh>(vertices, indices);
}