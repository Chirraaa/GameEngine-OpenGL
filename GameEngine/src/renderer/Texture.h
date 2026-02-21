#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
    unsigned int ID = 0;

    Texture() = default;
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
};