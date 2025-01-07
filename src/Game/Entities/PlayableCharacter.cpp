#include "PlayableCharacter.h"

#include <GLFW/glfw3.h>

#define MOVE_SPEED      150.0F

void PlayableCharacter::OnKeyPressed(int key)
{
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
    {
        m_input.x = 0.0F;
        m_input.y = -1.0F;
    }
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
    {
        m_input.x = 0.0F;
        m_input.y = 1.0F;
    }
    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
    {
        m_input.x = -1.0F;
        m_input.y = 0.0F;
    }
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
    {
        m_input.x = 1.0F;
        m_input.y = 0.0F;
    }
}

void PlayableCharacter::OnKeyReleased(int key)
{

}

void PlayableCharacter::Update(float deltaTime)
{
    AddPosition(m_input * MOVE_SPEED * deltaTime);
    AnimatedSprite::Update(deltaTime);
}

PlayableCharacter::PlayableCharacter(
    int divisions, float frameDuration,
    std::shared_ptr<Texture> &texture,
    glm::vec2 position,
    glm::vec2 size,
    glm::vec3 color)
        : AnimatedSprite(
            divisions,
            frameDuration,
            texture,
            position,
            size,
            color)
{
}
