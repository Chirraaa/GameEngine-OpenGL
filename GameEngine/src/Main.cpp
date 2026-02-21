#include "core/Window.h"
#include "core/Scene.h"
#include "core/UI.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Mesh.h"
#include "renderer/Model.h"
#include "renderer/Grid.h"
#include "renderer/Axes.h"
#include "camera/Camera.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

int main()
{
    Window  window(800, 600, "OpenGL Engine");
    Camera  camera(glm::vec3(0.0f, 3.0f, 8.0f));

    glfwSetWindowUserPointer(window.getNativeWindow(), &camera);
    glfwSetCursorPosCallback(window.getNativeWindow(), Camera::mouseCallback);
    glfwSetScrollCallback(window.getNativeWindow(), Camera::scrollCallback);

    Shader  litShader("shaders/default.vert", "shaders/default.frag");
    Shader  unlitShader("shaders/unlit.vert", "shaders/unlit.frag");
    Texture defaultTexture("Textures/wood.jpg");
    UI      ui(window.getNativeWindow());
    Grid    grid(20, 1.0f, &unlitShader);
    Axes    axes(1.5f, &unlitShader);

    std::vector<std::unique_ptr<Model>> importedModels;

    Scene scene;
    scene.lightPosition = glm::vec3(2.0f, 2.0f, 2.0f);
    scene.lightColor = glm::vec3(1.0f);

    glEnable(GL_DEPTH_TEST);

    float lastFrame = 0.0f;

    while (!window.shouldClose())
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(window.getNativeWindow(), deltaTime);

        scene.lightPosition.x = sin(currentFrame) * 3.0f;
        scene.lightPosition.z = cos(currentFrame) * 3.0f;

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(window.getAspectRatio());

        grid.draw(view, projection);
        axes.draw(view, projection);
        scene.draw(view, projection, camera.position);

        ui.begin();
        std::string importedPath = ui.render(scene, camera);
        ui.end();

        if (!importedPath.empty())
        {
            importedModels.push_back(std::make_unique<Model>(importedPath, &litShader, &defaultTexture));
            GameObject* obj = scene.add("Model_" + std::to_string(importedModels.size()),
                importedModels.back().get());
            obj->transform.position = glm::vec3(0.0f);
        }

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}