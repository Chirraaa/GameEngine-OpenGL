#include "Camera.h"

Camera::Camera(glm::vec3 startPosition)
    : position(startPosition),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    up(glm::vec3(0.0f, 1.0f, 0.0f)),
    yaw(-90.0f),
    pitch(0.0f),
    fov(45.0f),
    speed(2.5f),
    sensitivity(0.05f),
    m_lastX(0.0f),
    m_lastY(0.0f),
    m_firstMouse(true),
    m_active(true)
{
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static bool tabPressedLastFrame = false;
    bool tabPressed = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;
    if (tabPressed && !tabPressedLastFrame)
    {
        m_active = !m_active;
        glfwSetInputMode(window, GLFW_CURSOR, m_active ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
    tabPressedLastFrame = tabPressed;

    if (!m_active) return;

    float velocity = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += velocity * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= velocity * front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(front, up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(front, up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        position += velocity * up;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        position -= velocity * up;
}

void Camera::processMouse(float xoffset, float yoffset)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateVectors();
}

void Camera::processScroll(float yoffset)
{
    fov -= yoffset;
    if (fov < 1.0f)  fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}

void Camera::updateVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

void Camera::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam || !cam->m_active) return;

    if (!cam) return;

    if (cam->m_firstMouse)
    {
        cam->m_lastX = (float)xpos;
        cam->m_lastY = (float)ypos;
        cam->m_firstMouse = false;
    }

    float xoffset = (float)xpos - cam->m_lastX;
    float yoffset = cam->m_lastY - (float)ypos;
    cam->m_lastX = (float)xpos;
    cam->m_lastY = (float)ypos;

    cam->processMouse(xoffset, yoffset);
}

void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam) return;

    cam->processScroll((float)yoffset);
}