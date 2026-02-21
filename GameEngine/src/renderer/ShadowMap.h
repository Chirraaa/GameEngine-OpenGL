#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class ShadowMap
{
public:
    ShadowMap(int width = 2048, int height = 2048);
    ~ShadowMap();

    void bindForWriting() const;
    void bindForReading(unsigned int slot) const;

    glm::mat4 getLightSpaceMatrix(const glm::vec3& lightPos) const;

    unsigned int getDepthTexture() const { return m_depthTexture; }
    int getWidth()  const { return m_width; }
    int getHeight() const { return m_height; }

private:
    unsigned int m_FBO;
    unsigned int m_depthTexture;
    int          m_width;
    int          m_height;
};