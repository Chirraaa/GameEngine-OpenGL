#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../renderer/Shader.h"

class Grid
{
public:
    Grid(int halfSize, float step, Shader* shader);
    ~Grid();

    void draw(const glm::mat4& view, const glm::mat4& projection) const;

private:
    unsigned int m_VAO, m_VBO;
    int          m_vertexCount;
    Shader* m_shader;
};