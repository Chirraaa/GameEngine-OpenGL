#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    unsigned int vert = compileShader(loadFile(vertexPath), GL_VERTEX_SHADER);
    unsigned int frag = compileShader(loadFile(fragmentPath), GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vert);
    glAttachShader(ID, frag);
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << "\n";
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& m) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}

std::string Shader::loadFile(const std::string& path) const
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << path << "\n";
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

unsigned int Shader::compileShader(const std::string& source, GLenum type) const
{
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::"
            << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
            << "::COMPILATION_FAILED\n" << infoLog << "\n";
    }
    return shader;
}