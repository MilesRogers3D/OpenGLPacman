#pragma once

#include <memory>
#include "IO/ResourceManager.h"

class Sprite
{
public:
    explicit Sprite(
        std::shared_ptr<Texture>& texture,
        glm::vec2 position = glm::vec2(0.0F),
        glm::vec2 size = glm::vec2(10.0F, 10.0F),
        glm::vec3 color = glm::vec3(1.0F)
    );
    explicit Sprite(
        glm::vec2 position = glm::vec2(0.0F),
        glm::vec2 size = glm::vec2(10.0F, 10.0F),
        glm::vec3 color = glm::vec3(1.0F)
    );

    ~Sprite();

    void Draw();

    void SetRotation(float rotation);
    void AddRotation(float rotation);

private:
    void InitRenderData();

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Texture> m_texture;
    bool m_hasTexture = false;

    glm::vec2 m_position;
    float m_rotation = 0.0F;
    glm::vec2 m_pivot = glm::vec2(0.5F);
    glm::vec2 m_size;
    glm::vec3 m_color;

    glm::mat4 m_projMatrix = glm::ortho(
        0.0F,
        800.0F,
        600.0F,
        0.0F,
        -1.0F,
        1.0F
    );

    unsigned int m_quadVAO;
};
