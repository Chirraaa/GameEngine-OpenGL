#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
    unsigned int ID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;
    void setInt(const std::string& name, int value)           const;
    void setFloat(const std::string& name, float value)         const;
    void setMat4(const std::string& name, const glm::mat4& m)  const;
    void setVec3(const std::string& name, const glm::vec3& v)  const;

private:
    std::string  loadFile(const std::string& path) const;
    unsigned int compileShader(const std::string& source, GLenum type) const;
};