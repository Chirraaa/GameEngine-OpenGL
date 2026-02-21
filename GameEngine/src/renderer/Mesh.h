#pragma once

#include <glad/glad.h>
#include <vector>

struct Vertex
{
    float position[3];
    float normal[3];
    float texCoord[2];
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    Mesh(const std::vector<float>& vertices, int vertexStride);
    ~Mesh();

    void draw() const;

private:
    unsigned int m_VAO, m_VBO, m_EBO;
    int          m_indexCount;
    int          m_vertexCount;
    bool         m_indexed;
};