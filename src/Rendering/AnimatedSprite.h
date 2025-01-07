#pragma once

#include "Sprite.h"
#include <chrono>

class AnimatedSprite : public Sprite
{
public:
    explicit AnimatedSprite(
        int divisions,
        float frameDuration,
        std::shared_ptr<Texture>& texture,
        glm::vec2 position = glm::vec2(0.0F),
        glm::vec2 size = glm::vec2(10.0F, 10.0F),
        glm::vec3 color = glm::vec3(1.0F)
    );

    void Draw() override;
    void Update(float deltaTime) override;

private:
    int m_divisions;
    float m_frameDuration;
    float m_startTime;

    float GetCurrentTime();
};
