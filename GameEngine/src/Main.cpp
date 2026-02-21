#include "core/Window.h"
#include "core/Scene.h"
#include "core/UI.h"
#include "core/CallbackData.h"
#include "core/Ray.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Mesh.h"
#include "renderer/Model.h"
#include "renderer/Grid.h"
#include "renderer/Axes.h"
#include "renderer/Primitives.h"
#include "renderer/ShadowMap.h"
#include "camera/Camera.h"
#include "physics/PhysicsWorld.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

static void drawOutline(GameObject* obj, Shader& unlitShader,
    const glm::mat4& view, const glm::mat4& projection)
{
    if (!obj) return;

    glm::mat4 scaledModel = obj->transform.getModelMatrix();
    scaledModel = scaledModel * glm::scale(glm::mat4(1.0f), glm::vec3(1.05f));

    unlitShader.use();
    unlitShader.setMat4("model", scaledModel);
    unlitShader.setMat4("view", view);
    unlitShader.setMat4("projection", projection);
    unlitShader.setVec3("color", glm::vec3(1.0f, 0.65f, 0.0f));
}

int main()
{
    Window window(800, 600, "OpenGL Engine");
    Camera camera(glm::vec3(0.0f, 3.0f, 8.0f));

    CallbackData cbData{ &camera, &window };
    glfwSetWindowUserPointer(window.getNativeWindow(), &cbData);
    glfwSetCursorPosCallback(window.getNativeWindow(), Camera::mouseCallback);
    glfwSetScrollCallback(window.getNativeWindow(), Camera::scrollCallback);

    Shader litShader("shaders/default.vert", "shaders/default.frag");
    Shader unlitShader("shaders/unlit.vert", "shaders/unlit.frag");
    Shader depthShader("shaders/shadow.vert", "shaders/shadow.frag");

    Texture defaultTexture("Textures/wood.jpg");

    unsigned char whitePixel[] = { 255, 255, 255, 255 };
    unsigned int  whiteTexID;
    glGenTextures(1, &whiteTexID);
    glBindTexture(GL_TEXTURE_2D, whiteTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    Texture whiteTexture;
    whiteTexture.ID = whiteTexID;

    UI        ui(window.getNativeWindow());
    Grid      grid(20, 1.0f, &unlitShader);
    Axes      axes(1.5f, &unlitShader);
    ShadowMap shadowMap(2048, 2048);

    PhysicsWorld physics(0.0f);

    std::vector<std::unique_ptr<Model>> importedModels;
    std::vector<std::unique_ptr<Mesh>>  primitiveMeshes;

    Scene scene;
    scene.lightPosition = glm::vec3(5.0f, 10.0f, 5.0f);
    scene.lightColor = glm::vec3(1.0f);

    std::unique_ptr<Mesh> groundMesh = Primitives::plane(30.0f);
    Mesh* groundRaw = groundMesh.get();
    primitiveMeshes.push_back(std::move(groundMesh));
    scene.add("Ground", groundRaw, &litShader, &defaultTexture);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    int   primitiveCount = 0;
    float lastFrame = 0.0f;

    // Track mouse button state for picking
    bool  prevLeftDown = false;

    while (!window.shouldClose())
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(window.getNativeWindow(), deltaTime);
        physics.update(deltaTime);

        // Mouse picking — only when in UI mode (camera inactive)
        bool leftDown = glfwGetMouseButton(window.getNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        if (leftDown && !prevLeftDown && !camera.m_active)
        {
            double mx, my;
            glfwGetCursorPos(window.getNativeWindow(), &mx, &my);

            if (!ImGui::GetIO().WantCaptureMouse)
            {
                glm::mat4 view = camera.getViewMatrix();
                glm::mat4 projection = camera.getProjectionMatrix(window.getAspectRatio());
                Ray ray = Ray::fromMouse(mx, my, window.getWidth(), window.getHeight(), view, projection);
                scene.selectedIndex = scene.raycast(ray);
            }
        }
        prevLeftDown = leftDown;

        // Delete key shortcut
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_DELETE) == GLFW_PRESS
            && scene.selectedIndex >= 0)
        {
            scene.remove(scene.selectedIndex);
        }

        glm::mat4 lightSpaceMatrix = shadowMap.getLightSpaceMatrix(scene.lightPosition);

        // Pass 1 — depth
        shadowMap.bindForWriting();
        glEnable(GL_DEPTH_TEST);
        depthShader.use();
        depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        scene.drawDepthPass(depthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Pass 2 — main render with stencil
        glViewport(0, 0, window.getWidth(), window.getHeight());
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(window.getAspectRatio());

        shadowMap.bindForReading(1);
        litShader.use();
        litShader.setInt("ourTexture", 0);
        litShader.setInt("shadowMap", 1);
        litShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        grid.draw(view, projection);
        axes.draw(view, projection);

        // Draw scene — write 1 to stencil for selected object
        for (int i = 0; i < (int)scene.objects.size(); i++)
        {
            bool isSelected = (i == scene.selectedIndex);

            if (isSelected)
            {
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                glStencilMask(0xFF);
            }
            else
            {
                glStencilMask(0x00);
            }

            scene.objects[i]->draw(view, projection, scene.lightPosition, scene.lightColor,
                camera.position, lightSpaceMatrix, 1);
        }

        // Draw outline for selected object
        if (scene.selectedIndex >= 0 && scene.selectedIndex < (int)scene.objects.size())
        {
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);

            drawOutline(scene.objects[scene.selectedIndex].get(), unlitShader, view, projection);
            // Draw the scaled mesh
            auto& obj = scene.objects[scene.selectedIndex];
            if (obj)
            {
                glm::mat4 scaledModel = obj->transform.getModelMatrix()
                    * glm::scale(glm::mat4(1.0f), glm::vec3(1.05f));
                unlitShader.setMat4("model", scaledModel);
                // Re-draw the mesh geometry through the depth-fail region
                obj->drawDepth(unlitShader);
            }

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glEnable(GL_DEPTH_TEST);
        }

        ui.begin();
        UIResult uiResult = ui.render(scene, camera);
        ui.end();

        if (uiResult.deleteSelected)
            scene.remove(scene.selectedIndex);

        if (!uiResult.importedPath.empty())
        {
            importedModels.push_back(std::make_unique<Model>(
                uiResult.importedPath, &litShader, &defaultTexture));
            GameObject* obj = scene.add("Model_" + std::to_string(importedModels.size()),
                importedModels.back().get());
            obj->transform.position = glm::vec3(0.0f, 3.0f, 0.0f);
            obj->addRigidbody(1.0f, 0.6f);
            physics.add(obj->rigidbody.get(), &obj->transform, 1.0f);
        }

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