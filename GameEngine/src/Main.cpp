#include "core/Window.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Mesh.h"
#include "camera/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const std::vector<float> cubeVertices = {
    // positions          // tex coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int main()
{
    Window  window(800, 600, "OpenGL Engine");
    Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Register camera as GLFW user pointer so static callbacks can reach it
    glfwSetWindowUserPointer(window.getNativeWindow(), &camera);
    glfwSetCursorPosCallback(window.getNativeWindow(), Camera::mouseCallback);
    glfwSetScrollCallback(window.getNativeWindow(), Camera::scrollCallback);

    Shader  shader("shaders/default.vert", "shaders/default.frag");
    Texture texture("Textures/wood.jpg");
    Mesh    cube(cubeVertices, 5);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);

    float lastFrame = 0.0f;

    while (!window.shouldClose())
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(window.getNativeWindow(), deltaTime);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Spinning transform
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, currentFrame * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        trans = glm::scale(trans, glm::vec3(0.5f));

        shader.use();
        shader.setMat4("transform", trans);
        shader.setMat4("model", model);
        shader.setMat4("view", camera.getViewMatrix());

        shader.setMat4("projection", camera.getProjectionMatrix(window.getAspectRatio()));

        texture.bind(0);
        cube.draw();

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}