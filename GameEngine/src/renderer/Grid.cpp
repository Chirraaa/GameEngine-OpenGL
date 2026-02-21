#include "Grid.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

Grid::Grid(int halfSize, float step, Shader* shader)
    : m_shader(shader)
{
    std::vector<float> vertices;

    for (int i = -halfSize; i <= halfSize; i++)
    {
        float f = (float)i * step;
        float s = (float)halfSize * step;

        vertices.insert(vertices.end(), { f, 0.0f, -s, f, 0.0f,  s });
        vertices.insert(vertices.end(), { -s, 0.0f, f,  s, 0.0f,  f });
    }

    m_vertexCount = (int)vertices.size() / 3;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Grid::~Grid()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Grid::draw(const glm::mat4& view, const glm::mat4& projection) const
{
    m_shader->use();
    m_shader->setMat4("model", glm::mat4(1.0f));
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
    m_shader->setVec3("color", glm::vec3(0.3f));

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, m_vertexCount);
    glBindVertexArray(0);
}