#pragma once

#include "Sprite.h"

class TileSprite : public Sprite
{
public:
    explicit TileSprite(
        glm::ivec2 divisions,
        std::shared_ptr<Texture>& texture,
        glm::vec2 position = glm::vec2(0.0F),
        glm::vec2 size = glm::vec2(10.0F, 10.0F),
        glm::vec3 color = glm::vec3(1.0F)
    );

    void SetCurrentTileIndex(int index);
    void Draw(std::shared_ptr<Camera>& camera) override;
    void Update(float deltaTime) override;

    int GetTileAmount();

private:
    glm::ivec2 m_divisions = glm::ivec2(1, 1);
    int m_tileIndex = 0;
};
