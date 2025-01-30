#include "Pacman.h"
#include "Core/Scene/Components.h"

#include <GLFW/glfw3.h>

#define PACMAN_POSITION            glm::vec2(425.0F, 630.0F)
#define PACMAN_SIZE                52.0F
#define PACMAN_MOVE_SPEED          150.0F
#define PACMAN_ANIM_FRAME_TIME     0.1F

void Pacman::OnKeyPressed(int key)
{
    auto& transform = m_entity.GetComponent<TransformComponent>();
    
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
    {
        m_input.x = 0.0F;
        m_input.y = -1.0F;
        
        transform.SetRotation(-90.0F);
    }
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
    {
        m_input.x = 0.0F;
        m_input.y = 1.0F;
        
        transform.SetRotation(90.0F);
    }
    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
    {
        m_input.x = -1.0F;
        m_input.y = 0.0F;
        
        transform.SetRotation(-180.0F);
    }
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
    {
        m_input.x = 1.0F;
        m_input.y = 0.0F;
        
        transform.SetRotation(0.0F);
    }
}

void Pacman::OnKeyReleased(int key)
{

}

Pacman::Pacman(const std::shared_ptr<Scene>& scene)
{
    m_scene = scene;
    m_entity = m_scene.lock()->CreateEntity("Pacman");
    
    auto& transform = m_entity.GetComponent<TransformComponent>();
    transform.Position = glm::vec2(PACMAN_POSITION);
    transform.Size = glm::vec2(PACMAN_SIZE);
    
    auto pacmanTex = ResourceManager::LoadTexture(
        "res/sprites/pacman.png",
        "Pacman"
    );
    
    m_entity.AddComponent<SpriteRendererComponent>(
        pacmanTex,
        glm::vec4(1.0F)
    );
    m_entity.AddComponent<FlipbookComponent>(3, PACMAN_ANIM_FRAME_TIME);
    
    auto& boxCollider = m_entity.AddComponent<BoxColliderComponent>();
    boxCollider.DrawDebugCollision = true;
}

void Pacman::OnUpdate(float deltaTime)
{
    auto& transform = m_entity.GetComponent<TransformComponent>();
    transform.Position += m_input * PACMAN_MOVE_SPEED * deltaTime;
}
