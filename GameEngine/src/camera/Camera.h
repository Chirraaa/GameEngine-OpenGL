#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float yaw;
    float pitch;
    float fov;
    float speed;
    float sensitivity;

    Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 3.0f));

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    void processKeyboard(GLFWwindow* window, float deltaTime);
    void processMouse(float xoffset, float yoffset);
    void processScroll(float yoffset);

    // Static GLFW callbacks — register via glfwSetWindowUserPointer(window, &camera)
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    float m_lastX;
    float m_lastY;
    bool  m_firstMouse;

    bool m_active;

    void updateVectors();
};