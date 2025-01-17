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

    void DrawCollision(std::shared_ptr<Camera>& camera);

    void SetPosition(glm::vec2 position);
    void AddPosition(glm::vec2 position);

    void SetRotation(float rotation);
    void AddRotation(float rotation);

    void SetScale(glm::vec2 scale);

    void SetFlipHorizontal(bool flip);
    void SetFlipVertical(bool flip);
    void SetFlipDiagonal(bool flip);

    void ResetTransform();

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

    bool m_flipHorizontal = false;
    bool m_flipVertical = false;
    bool m_flipDiagonal = false;

    unsigned int m_quadVAO;

private:
    struct BoxCollider
    {
        glm::vec2 Origin = { 0.0F, 0.0F };
        glm::vec2 Size = { 1.0F, 1.0F };
    };

    struct ColliderData
    {
        glm::vec2 WorldOrigin;
        glm::vec2 WorldSize;
    };

    BoxCollider m_boxCollider;

    bool m_hasCollision = false;
    bool m_drawCollision = false;

public:
    void SetCollisionEnabled(bool enable);
    void SetDrawCollision(bool enable);

    ColliderData GetColliderData();
};
