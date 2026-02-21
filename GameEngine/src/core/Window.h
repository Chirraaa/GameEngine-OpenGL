#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers() const;
    void pollEvents() const;

    GLFWwindow* getNativeWindow() const { return m_window; }
    int   getWidth()       const { return m_width; }
    int   getHeight()      const { return m_height; }
    float getAspectRatio() const { return (float)m_width / (float)m_height; }

private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
};