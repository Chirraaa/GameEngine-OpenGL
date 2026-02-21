#pragma once

#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "../camera/Camera.h"

class UI
{
public:
    UI(GLFWwindow* window);
    ~UI();

    void begin() const;
    void render(Scene& scene, Camera& camera) const;
    void end() const;
};