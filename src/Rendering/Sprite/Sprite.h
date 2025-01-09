#pragma once

#include "IO/ResourceManager.h"
#include "Rendering/Camera.h"
#include <memory>

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

    virtual void Draw(std::shared_ptr<Camera>& camera);
    virtual void Update(float deltaTime);

    void SetPosition(glm::vec2 position);
    void AddPosition(glm::vec2 position);

    void SetRotation(float rotation);
    void AddRotation(float rotation);

protected:
    void InitRenderData();

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Texture> m_texture;
    bool m_hasTexture = false;

    glm::vec2 m_position;
    float m_rotation = 0.0F;
    glm::vec2 m_pivot = glm::vec2(0.5F);
    glm::vec2 m_size;
    glm::vec3 m_color;

    unsigned int m_quadVAO;
};
