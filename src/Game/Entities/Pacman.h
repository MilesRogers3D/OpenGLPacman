#pragma once

#include "Rendering/Sprite/AnimatedSprite.h"
#include "Core/Scene/Entity.h"

class Pacman
{
public:
    explicit Pacman(
        const std::shared_ptr<Scene>& scene
    );
    
    void OnUpdate(float deltaTime);

    void OnKeyPressed(int key);
    void OnKeyReleased(int key);

private:
    Entity m_entity;
    glm::vec2 m_input = glm::vec2(0.0F);
    
    std::weak_ptr<Scene> m_scene;
};
