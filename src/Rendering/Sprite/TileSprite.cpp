#include "TileSprite.h"
#include "IO/Log.h"

TileSprite::TileSprite(
    glm::ivec2 divisions,
    std::shared_ptr<Texture> &texture,
    glm::vec2 position,
    glm::vec2 size,
    glm::vec3 color)
        : Sprite(
            texture,
            position,
            size,
            color)
{
    m_divisions = divisions;
}

void TileSprite::SetCurrentTileIndex(int index)
{
    int maxPossible = m_divisions.x * m_divisions.y;

    if (index > maxPossible)
    {
        Log::Info(
        "Index %i out of tilemap range! Wrapping to %i.",
            index,
            maxPossible
        );
        m_tileIndex = maxPossible;
        return;
    }

    m_tileIndex = index;
}

void TileSprite::Draw(std::shared_ptr<Camera> &camera)
{
    m_shader->Use();
    m_shader->SetInt("xDivisions", m_divisions.x);
    m_shader->SetInt("yDivisions", m_divisions.y);
    m_shader->SetInt("frame", m_tileIndex);

    Sprite::Draw(camera);

    m_shader->SetInt("xDivisions", 1);
    m_shader->SetInt("yDivisions", 1);
    m_shader->SetInt("frame", 0);
}

void TileSprite::Update(float deltaTime)
{
    Sprite::Update(deltaTime);
}

int TileSprite::GetTileAmount()
{
    return m_divisions.x + m_divisions.y;
}
