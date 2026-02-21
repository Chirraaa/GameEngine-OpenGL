#include "core/Window.h"
#include "core/Scene.h"
#include "core/UI.h"
#include "core/CallbackData.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Mesh.h"
#include "renderer/Model.h"
#include "renderer/Grid.h"
#include "renderer/Axes.h"
#include "renderer/Primitives.h"
#include "camera/Camera.h"
#include "physics/PhysicsWorld.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

int main()
{
    Window  window(800, 600, "OpenGL Engine");
    Camera  camera(glm::vec3(0.0f, 3.0f, 8.0f));

    CallbackData cbData{ &camera, &window };

    glfwSetWindowUserPointer(window.getNativeWindow(), &cbData);
    glfwSetCursorPosCallback(window.getNativeWindow(), Camera::mouseCallback);
    glfwSetScrollCallback(window.getNativeWindow(), Camera::scrollCallback);

    Shader  litShader("shaders/default.vert", "shaders/default.frag");
    Shader  unlitShader("shaders/unlit.vert", "shaders/unlit.frag");
    UI      ui(window.getNativeWindow());
    Grid    grid(20, 1.0f, &unlitShader);
    Axes    axes(1.5f, &unlitShader);

    // 1x1 white texture for unlit primitives that still receive lighting
    unsigned char whitePixel[] = { 255, 255, 255, 255 };
    unsigned int  whiteTexID;
    glGenTextures(1, &whiteTexID);
    glBindTexture(GL_TEXTURE_2D, whiteTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    Texture whiteTexture;
    whiteTexture.ID = whiteTexID;

    Texture defaultTexture("Textures/wood.jpg");

    PhysicsWorld physics(0.0f);

    std::vector<std::unique_ptr<Model>> importedModels;
    std::vector<std::unique_ptr<Mesh>>  primitiveMeshes;

    Scene scene;
    scene.lightPosition = glm::vec3(2.0f, 4.0f, 2.0f);
    scene.lightColor = glm::vec3(1.0f);

    glEnable(GL_DEPTH_TEST);

    int   primitiveCount = 0;
    float lastFrame = 0.0f;

    while (!window.shouldClose())
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(window.getNativeWindow(), deltaTime);
        physics.update(deltaTime);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(window.getAspectRatio());

        grid.draw(view, projection);
        axes.draw(view, projection);
        scene.draw(view, projection, camera.position);

        ui.begin();
        UIResult uiResult = ui.render(scene, camera);
        ui.end();

        // Handle model import
        if (!uiResult.importedPath.empty())
        {
            importedModels.push_back(std::make_unique<Model>(uiResult.importedPath, &litShader, &defaultTexture));
            GameObject* obj = scene.add("Model_" + std::to_string(importedModels.size()),
                importedModels.back().get());
            obj->transform.position = glm::vec3(0.0f, 5.0f, 0.0f);
            obj->addRigidbody(1.0f, 0.6f);
            physics.add(obj->rigidbody.get(), &obj->transform, 1.0f);
        }

        // Handle primitive spawn
        if (uiResult.spawnPrimitive != PrimitiveType::None)
        {
            std::unique_ptr<Mesh> mesh;
            std::string           name;

            switch (uiResult.spawnPrimitive)
            {
            case PrimitiveType::Cube:        mesh = Primitives::cube();        name = "Cube";        break;
            case PrimitiveType::Sphere:      mesh = Primitives::sphere();      name = "Sphere";      break;
            case PrimitiveType::Tetrahedron: mesh = Primitives::tetrahedron(); name = "Tetrahedron"; break;
            case PrimitiveType::Capsule:     mesh = Primitives::capsule();     name = "Capsule";     break;
            case PrimitiveType::Plane:       mesh = Primitives::plane();       name = "Plane";       break;
            case PrimitiveType::Circle:      mesh = Primitives::circle();      name = "Circle";      break;
            default: break;
            }

            if (mesh)
            {
                primitiveCount++;
                Mesh* rawMesh = mesh.get();
                primitiveMeshes.push_back(std::move(mesh));

                GameObject* obj = scene.add(name + "_" + std::to_string(primitiveCount),
                    rawMesh, &litShader, &whiteTexture);
                obj->transform.position = glm::vec3(0.0f, 3.0f, 0.0f);
                obj->addRigidbody(1.0f, 0.5f);
                physics.add(obj->rigidbody.get(), &obj->transform, 0.5f);
            }
        }

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}