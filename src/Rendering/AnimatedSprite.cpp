#include "AnimatedSprite.h"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<float> FSec;

#include <GLFW/glfw3.h>

AnimatedSprite::AnimatedSprite(
    int divisions,
    float frameDuration,
    std::shared_ptr<Texture> &texture,
    glm::vec2 position,
    glm::vec2 size,
    glm::vec3 color)
        : Sprite (
            texture,
            position,
            size,
            color)
{
    m_divisions = divisions;
    m_frameDuration = frameDuration;

    m_startTime = GetCurrentTime();
}

void AnimatedSprite::Draw()
{
    m_shader->Use();

    m_shader->SetInt("divisions", m_divisions);

    // Get elapsed time in seconds
    double elapsedTime = glfwGetTime();

    int frame = (int)(elapsedTime / m_frameDuration) % m_divisions;
    m_shader->SetInt("frame", frame);

    Sprite::Draw();

    m_shader->SetInt("divisions", 1);
    m_shader->SetInt("frame", 0);
}

float AnimatedSprite::GetCurrentTime()
{
    auto now = std::chrono::steady_clock::now();
    auto timeInSeconds = std::chrono::duration_cast<std::chrono::duration<float>>(
        now.time_since_epoch()
    );
    return timeInSeconds.count();
}

void AnimatedSprite::Update(float deltaTime)
{
    Sprite::Update(deltaTime);
}
