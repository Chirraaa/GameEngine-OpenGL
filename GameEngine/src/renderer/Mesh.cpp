#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices, int vertexStride)
{
    m_vertexCount = (int)vertices.size() / vertexStride;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW);

    // Position attribute (location 0) — vec3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        vertexStride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coords attribute (location 1) — vec2
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        vertexStride * sizeof(float),
        (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Mesh::draw() const
{
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}