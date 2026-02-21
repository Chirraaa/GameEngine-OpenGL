#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, NULL);
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL();
    glViewport(0, 0, m_width, m_height);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

void Window::pollEvents() const
{
    glfwPollEvents();
}