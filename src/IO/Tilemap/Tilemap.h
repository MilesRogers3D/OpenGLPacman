#pragma once

#include "Rendering/Camera.h"
#include "Rendering/Sprite/TileSprite.h"
#include "Core/Scene/Entity.h"
#include "Core/Scene/Components.h"

#include <string>
#include <vector>
#include <memory>

struct TilemapInput
{
    glm::ivec2 Dimensions = { 1, 1 };
    std::shared_ptr<Texture> Texture;
};

class Tilemap
{
public:
    Tilemap(
        const std::shared_ptr<Scene>& scene,
        const char* path,
        const std::vector<TilemapInput>& tilemaps,
        int pixelsPerUnit = 8
    );

    void Draw(std::shared_ptr<Camera>& camera);

private:
    struct Tile
    {
        int ID = 0;
        bool FlipVertical = false;
        bool FlipHorizontal = false;
        bool FlipDiagonal = false;
    };

    struct TileLayer
    {
        std::string Name;
        int Width = 0;
        int Height = 0;
        std::vector<Entity> TileEntities;
    };

    struct Tileset
    {
        int FirstGID = 0;
        int Length = 0;
        TilemapInput TilemapData;
    };

    std::vector<Tileset> m_tileSets;
    std::vector<TileLayer> m_tileLayers;
    
    std::weak_ptr<Scene> m_scene;

    int m_pixelsPerUnit;
    int m_tileSize;

    float m_tileFootprint;
};
