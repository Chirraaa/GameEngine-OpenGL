#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, NULL);
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL();

    glfwGetFramebufferSize(m_window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }
void Window::swapBuffers()  const { glfwSwapBuffers(m_window); }
void Window::pollEvents()   const { glfwPollEvents(); }

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!self) return;
    self->m_width = width;
    self->m_height = height;
    glViewport(0, 0, width, height);
}