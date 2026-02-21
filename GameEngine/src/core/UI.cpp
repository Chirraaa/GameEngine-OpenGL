#include "UI.h"
#include <windows.h>
#include <commdlg.h>

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

UIResult UI::render(Scene& scene, Camera& camera) const
{
    UIResult result{ "", PrimitiveType::None, false };

    ImGui::Begin("Debug");

    if (ImGui::Button("Import Model"))
        result.importedPath = openFileDialog();

    if (ImGui::CollapsingHeader("Add Primitive"))
    {
        if (ImGui::Button("Cube"))        result.spawnPrimitive = PrimitiveType::Cube;
        ImGui::SameLine();
        if (ImGui::Button("Sphere"))      result.spawnPrimitive = PrimitiveType::Sphere;
        ImGui::SameLine();
        if (ImGui::Button("Tetrahedron")) result.spawnPrimitive = PrimitiveType::Tetrahedron;
        if (ImGui::Button("Capsule"))     result.spawnPrimitive = PrimitiveType::Capsule;
        ImGui::SameLine();
        if (ImGui::Button("Plane"))       result.spawnPrimitive = PrimitiveType::Plane;
        ImGui::SameLine();
        if (ImGui::Button("Circle"))      result.spawnPrimitive = PrimitiveType::Circle;
    }

    if (ImGui::CollapsingHeader("Light"))
    {
        ImGui::DragFloat3("Position", &scene.lightPosition.x, 0.1f);
        ImGui::ColorEdit3("Color", &scene.lightColor.x);
    }

    if (ImGui::CollapsingHeader("Camera"))
    {
        ImGui::DragFloat("Speed", &camera.speed, 0.1f, 0.1f, 20.0f);
        ImGui::DragFloat("Sensitivity", &camera.sensitivity, 0.01f, 0.01f, 1.0f);
        ImGui::Text("Position: (%.2f, %.2f, %.2f)",
            camera.position.x, camera.position.y, camera.position.z);
    }

    if (ImGui::CollapsingHeader("Objects"))
    {
        for (int i = 0; i < (int)scene.objects.size(); i++)
        {
            auto& obj = scene.objects[i];

            bool isSelected = (scene.selectedIndex == i);

            if (isSelected)
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.5f, 0.9f, 1.0f));

            bool open = ImGui::TreeNodeEx(obj->name.c_str(),
                isSelected ? ImGuiTreeNodeFlags_Selected : 0);

            if (isSelected)
                ImGui::PopStyleColor();

            if (ImGui::IsItemClicked())
                scene.selectedIndex = isSelected ? -1 : i;

            if (open)
            {
                ImGui::DragFloat3("Position", &obj->transform.position.x, 0.1f);
                ImGui::DragFloat3("Rotation", &obj->transform.rotation.x, 1.0f);
                ImGui::DragFloat3("Scale", &obj->transform.scale.x, 0.01f);

                if (obj->rigidbody)
                {
                    ImGui::Separator();
                    ImGui::Text("Rigidbody");
                    ImGui::DragFloat("Mass", &obj->rigidbody->mass, 0.1f, 0.1f, 100.0f);
                    ImGui::DragFloat("Restitution", &obj->rigidbody->restitution, 0.01f, 0.0f, 1.0f);
                    ImGui::Checkbox("Static", &obj->rigidbody->isStatic);
                    ImGui::Text("Velocity: (%.2f, %.2f, %.2f)",
                        obj->rigidbody->velocity.x,
                        obj->rigidbody->velocity.y,
                        obj->rigidbody->velocity.z);
                    if (ImGui::Button("Apply Upward Impulse"))
                        obj->rigidbody->applyForce(glm::vec3(0.0f, 300.0f, 0.0f));
                    if (ImGui::Button("Reset Velocity"))
                        obj->rigidbody->reset();
                }

                ImGui::Separator();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
                if (ImGui::Button("Delete"))
                {
                    scene.selectedIndex = i;
                    result.deleteSelected = true;
                }
                ImGui::PopStyleColor();

                ImGui::TreePop();
            }
        }
    }

    ImGui::End();
    return result;
}

void UI::end() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::string UI::openFileDialog() const
{
    char path[MAX_PATH] = "";
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "3D Models\0*.obj;*.fbx;*.gltf;*.glb\0All Files\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn))
        return std::string(path);
    return "";
}