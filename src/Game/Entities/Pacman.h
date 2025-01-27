#pragma once

#include "Rendering/Sprite/AnimatedSprite.h"
#include "Core/Scene/Scene.h"

class Pacman : public AnimatedSprite
{
public:
    explicit Pacman(
        int divisions,
        float frameDuration,
        std::shared_ptr<Texture>& texture,
        glm::vec2 position = glm::vec2(0.0F),
        glm::vec2 size = glm::vec2(10.0F, 10.0F),
        glm::vec3 color = glm::vec3(1.0F)
    );

    void OnKeyPressed(int key);
    void OnKeyReleased(int key);

    void Update(float deltaTime) override;

private:
    glm::vec2 m_input = glm::vec2(0.0F);
};
