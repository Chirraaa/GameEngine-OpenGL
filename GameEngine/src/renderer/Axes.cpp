#include "Axes.h"
#include <glm/gtc/matrix_transform.hpp>

Axes::Axes(float length, Shader* shader)
    : m_shader(shader)
{
    float v[] = {
        0.0f, 0.0f, 0.0f,  length, 0.0f,   0.0f,   // X
        0.0f, 0.0f, 0.0f,  0.0f,   length,  0.0f,   // Y
        0.0f, 0.0f, 0.0f,  0.0f,   0.0f,    length  // Z
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Axes::~Axes()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Axes::draw(const glm::mat4& view, const glm::mat4& projection) const
{
    m_shader->use();
    m_shader->setMat4("model", glm::mat4(1.0f));
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);

    glBindVertexArray(m_VAO);

    m_shader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
    glDrawArrays(GL_LINES, 0, 2);

    m_shader->setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
    glDrawArrays(GL_LINES, 2, 2);

    m_shader->setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 4, 2);

    glBindVertexArray(0);
}