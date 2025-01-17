#pragma once

#include "Rendering/Texture.h"

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
    TransformComponent(
        glm::vec2 position,
        float rotation,
        glm::vec2 size)
    {
        Position = position;
        Rotation = rotation;
        Size = size;
    }
};

struct SpriteRendererComponent
{
    int TextureID = 0;
    glm::vec4 ColorTint = { 1.0F, 1.0F, 1.0F, 1.0F };

    bool FlipHorizontal = false;
    bool FlipVertical = false;
    bool FlipDiagonal = false;

    explicit SpriteRendererComponent(
        const std::shared_ptr<Texture>& texture,
        glm::vec4 color)
    {
        TextureID = (int)texture->ID;
        ColorTint = color;
    }

    explicit SpriteRendererComponent(
        glm::vec4 color)
    {
        ColorTint = color;
    }
};

struct BoxColliderComponent
{
    // Position relative to transform pivot point
    glm::vec2 Position;

    // Size relative to transform size
    glm::vec2 Size;

    explicit BoxColliderComponent(
        glm::vec2 position,
        glm::vec2 size)
            : Position(position),
              Size(size) {};
};

