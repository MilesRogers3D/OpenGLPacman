#include "Entity.h"
#include "Components.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Entity::Entity(
    entt::entity handle,
    const std::shared_ptr<Scene>& scene)
        : m_handle(handle), m_scene(scene)
{
}

void Entity::OnGUIDraw()
{
    // Draw transform
    auto& transform = GetComponent<TransformComponent>();
    
    float position[2] =
    {
        transform.Position.x,
        transform.Position.y
    };
    float rotation = transform.Rotation;
    float size[2] =
    {
        transform.Size.x,
        transform.Size.y
    };
    
    ImGui::InputFloat2("Transform", position);
    ImGui::InputFloat("Rotation", &rotation);
    ImGui::InputFloat2("Size", size);
    
    transform.Position = glm::vec2(position[0], position[1]);
    transform.Rotation = rotation;
    transform.Size = glm::vec2(size[0], size[1]);
}
