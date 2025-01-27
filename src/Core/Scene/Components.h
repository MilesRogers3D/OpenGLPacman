#pragma once

#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/Font/BitmapFont.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <utility>

struct NameComponent
{
    std::string Name;

    explicit NameComponent(std::string name)
        : Name(std::move(name)) {};
};

enum ETag
{
    Default,
    Player,
    Enemy
};

struct TagComponent
{
    ETag Tag = ETag::Default;

    TagComponent() = default;
    explicit TagComponent(ETag tag)
        : Tag(tag) {};
};

struct TransformComponent
{
    glm::vec2 Position = { 0.0F, 0.0F };
    float Rotation = 0.0F;
    glm::vec2 Size = { 100.0F, 100.0F };

    glm::vec2 Pivot = { 0.5F, 0.5F };
    
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(
        glm::vec2 position,
        float rotation,
        glm::vec2 size)
            : Position(position),
              Rotation(rotation),
              Size(size) {}
};

struct SpriteRendererComponent
{
    glm::vec4 ColorTint = { 1.0F, 1.0F, 1.0F, 1.0F };

    bool FlipHorizontal = false;
    bool FlipVertical = false;
    bool FlipDiagonal = false;

    SpriteRendererComponent() = default;
    
    SpriteRendererComponent(
        const std::shared_ptr<Texture>& texture,
        glm::vec4 color)
    {
        m_spriteTexture = texture;
        m_hasTexture = true;
        
        ColorTint = color;
    }
    
    SpriteRendererComponent(glm::vec4 color)
    {
        ColorTint = color;
    }
    
    void SetTexture(const std::shared_ptr<Texture>& texture)
    {
        m_spriteTexture = texture;
        m_hasTexture = true;
    }
    
    const std::weak_ptr<Texture>& GetTexture() const
    {
        return m_spriteTexture;
    }
    
    bool HasTexture() const
    {
        return m_hasTexture;
    }
    
private:
    std::weak_ptr<Texture> m_spriteTexture;
    bool m_hasTexture = false;
};

struct FlipbookComponent
{
    int Divisions = 1;
    float FrameDuration = 0.15F;
    
    FlipbookComponent()
    {
        m_initialTime = (float)glfwGetTime();
        m_currentTime = (float)glfwGetTime();
    }
    FlipbookComponent(
        int divisions,
        float frameDuration)
            : FlipbookComponent()
    {
        Divisions = divisions;
        FrameDuration = frameDuration;
    }
    FlipbookComponent(
        const FlipbookComponent& other)
            : FlipbookComponent()
    {
        Divisions = other.Divisions;
        FrameDuration = other.FrameDuration;
    }
    
    float GetCurrentTime()
    {
        m_currentTime = (float)glfwGetTime() - m_initialTime;
        return m_currentTime;
    }
    
private:
    float m_initialTime = 0.0F;
    float m_currentTime = 0.0F;
};

struct TileComponent
{
    glm::ivec2 Divisions = glm::ivec2(1, 1);
    int TileIndex = 0;
    
    int GetTileAmount() const
    {
        return Divisions.x * Divisions.y;
    }
    
    TileComponent() = default;
    TileComponent(int xDivisions, int yDivisions)
    {
        Divisions = glm::ivec2(xDivisions, yDivisions);
    }
    TileComponent(const glm::ivec2& divisions)
    {
        Divisions = divisions;
    }
};

struct FontRendererComponent
{
    std::string Text;
    std::weak_ptr<BitmapFont> Font;
    glm::vec4 Color = glm::vec4(1.0F);
    
    float FontSize = 8.0F;
    
    FontRendererComponent() = default;
    FontRendererComponent(
        std::string text,
        const std::shared_ptr<BitmapFont>& font,
        float size)
            : Text(std::move(text)),
              Font(font),
              FontSize(size) {}
};

struct BoxColliderComponent
{
    // Position relative to transform pivot point
    glm::vec2 Position = glm::vec2(0.0F);

    // Size relative to transform size
    glm::vec2 Size = glm::vec2(1.0F);
    
    bool DrawDebugCollision = false;

    BoxColliderComponent() = default;
    BoxColliderComponent(
        glm::vec2 position,
        glm::vec2 size)
            : Position(position),
              Size(size) {};
};

