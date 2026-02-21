#include "core/Window.h"
#include "core/Scene.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Mesh.h"
#include "camera/Camera.h"

#include <glm/glm.hpp>

static const std::vector<float> cubeVertices = {
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
    Camera  camera(glm::vec3(0.0f, 0.0f, 5.0f));

    glfwSetWindowUserPointer(window.getNativeWindow(), &camera);
    glfwSetCursorPosCallback(window.getNativeWindow(), Camera::mouseCallback);
    glfwSetScrollCallback(window.getNativeWindow(), Camera::scrollCallback);

    Shader  shader("shaders/default.vert", "shaders/default.frag");
    Texture texture("Textures/wood.jpg");
    Mesh    cubeMesh(cubeVertices, 5);

    Scene scene;

    GameObject* cube1 = scene.add("Cube1", &cubeMesh, &shader, &texture);
    cube1->transform.position = glm::vec3(-1.5f, 0.0f, 0.0f);
    cube1->transform.scale = glm::vec3(1.0f);

    GameObject* cube2 = scene.add("Cube2", &cubeMesh, &shader, &texture);
    cube2->transform.position = glm::vec3(1.5f, 0.0f, 0.0f);
    cube2->transform.scale = glm::vec3(0.5f);

    glEnable(GL_DEPTH_TEST);

    float lastFrame = 0.0f;

    while (!window.shouldClose())
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(window.getNativeWindow(), deltaTime);

        cube1->transform.rotation.y = currentFrame * 90.0f;
        cube2->transform.rotation.y = currentFrame * -60.0f;
        cube2->transform.rotation.x = currentFrame * 45.0f;

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.draw(
            camera.getViewMatrix(),
            camera.getProjectionMatrix(window.getAspectRatio())
        );

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}