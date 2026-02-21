#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Axes
{
public:
    Axes(float length, Shader* shader);
    ~Axes();

    void draw(const glm::mat4& view, const glm::mat4& projection) const;

private:
    unsigned int m_VAO, m_VBO;
    Shader* m_shader;
};