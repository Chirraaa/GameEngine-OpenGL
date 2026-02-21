#include "UI.h"

UI::UI(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

UI::~UI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::begin() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::render(Scene& scene, Camera& camera) const
{
    ImGui::Begin("Debug");

    if (ImGui::CollapsingHeader("Light"))
    {
        ImGui::DragFloat3("Position", &scene.lightPosition.x, 0.1f);
        ImGui::ColorEdit3("Color", &scene.lightColor.x);
    }

    if (ImGui::CollapsingHeader("Camera"))
    {
        ImGui::DragFloat("Speed", &camera.speed, 0.1f, 0.1f, 20.0f);
        ImGui::DragFloat("Sensitivity", &camera.sensitivity, 0.01f, 0.01f, 1.0f);
        ImGui::Text("Position: (%.2f, %.2f, %.2f)", camera.position.x, camera.position.y, camera.position.z);
    }

    if (ImGui::CollapsingHeader("Objects"))
    {
        for (auto& obj : scene.objects)
        {
            if (ImGui::TreeNode(obj->name.c_str()))
            {
                ImGui::DragFloat3("Position", &obj->transform.position.x, 0.1f);
                ImGui::DragFloat3("Rotation", &obj->transform.rotation.x, 1.0f);
                ImGui::DragFloat3("Scale", &obj->transform.scale.x, 0.01f);
                ImGui::TreePop();
            }
        }
    }

    ImGui::End();
}

void UI::end() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}