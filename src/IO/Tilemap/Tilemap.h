#pragma once

#include "Rendering/Camera.h"
#include "Rendering/Sprite/TileSprite.h"

#include <string>
#include <vector>
#include <memory>

class Tilemap
{
public:
    Tilemap(
        const char* path,
        const std::vector<std::shared_ptr<TileSprite>>& tileSprites,
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
        std::vector<Tile> Tiles;
    };

    struct Tileset
    {
        int FirstGID = 0;
        int Length = 0;
        std::shared_ptr<TileSprite> TileSprite;
    };

    std::vector<Tileset> m_tileSets;
    std::vector<TileLayer> m_tileLayers;

    int m_pixelsPerUnit;
    int m_tileSize;

    float m_tileFootprint;
};
