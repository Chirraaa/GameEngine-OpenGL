#pragma once

#include <glad/glad.h>
#include <vector>

class Mesh
{
public:
    Mesh(const std::vector<float>& vertices, int vertexStride);
    ~Mesh();

    void draw() const;

private:
    unsigned int m_VAO, m_VBO;
    int m_vertexCount;
};