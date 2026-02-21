#pragma once

#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "../camera/Camera.h"
#include <string>

enum class PrimitiveType { None, Cube, Sphere, Tetrahedron, Capsule, Plane, Circle };

struct UIResult
{
    std::string   importedPath;
    PrimitiveType spawnPrimitive;
    bool          deleteSelected;
};

class UI
{
public:
    UI(GLFWwindow* window);
    ~UI();

    void     begin() const;
    UIResult render(Scene& scene, Camera& camera) const;
    void     end() const;

private:
    std::string openFileDialog() const;
};