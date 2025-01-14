#include "Tilemap.h"

#include "IO/Log.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const unsigned FLIPPED_HORIZONTALLY_FLAG  = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG    = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG    = 0x20000000;
const unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000;

Tilemap::Tilemap(
    const char* path,
    const std::vector<std::shared_ptr<TileSprite>>& tileSprites,
    int pixelsPerUnit)
{
    m_pixelsPerUnit = pixelsPerUnit;

    // Parse JSON file
    std::ifstream file(path);
    json data = json::parse(file);

    // Verify tile dimensions are square, populate m_tileSize field
    int tileWidth = data["tilewidth"];
    int tileHeight = data["tileheight"];

    if (tileWidth != tileHeight)
    {
        Log::Critical(
            "Error loading tilemap! Non-square tiles not supported!"
        );
    }

    m_tileSize = tileHeight;

    // Set tile footprint in all sprites
    m_tileFootprint = (float)m_tileSize
        / (float)m_pixelsPerUnit * 100.0F;

    // Get first GIDs from each tile set
    if (data["tilesets"].size() != tileSprites.size())
    {
        Log::Critical(
            "Incorrect amount of TileSprites loaded in texture array!"
        );
    }

    // Get tilesets and match them to their TileSprites
    m_tileSets.clear();
    int tilesetIndex = 0;

    for (json::iterator it = data["tilesets"].begin();
         it != data["tilesets"].end();
         it++)
    {
        Tileset tileset = {};
        tileset.FirstGID = it.value()["firstgid"];
        tileset.TileSprite = tileSprites[tilesetIndex];
        tileset.Length = tileSprites[tilesetIndex]->GetTileAmount();

        m_tileSets.emplace_back(tileset);

        tilesetIndex++;
    }

    // Get layers
    m_tileLayers.clear();

    for (json::iterator it = data["layers"].begin();
         it != data["layers"].end();
         it++)
    {
        TileLayer layer = {};

        layer.Name = it.value()["name"];
        layer.Width = it.value()["width"];
        layer.Height = it.value()["height"];

        std::vector<Tile> tiles;
        tiles.clear();

        std::vector<uint32_t> entries = it.value()["data"];
        auto* tileData = reinterpret_cast<unsigned char*>(entries.data());

        int tileIndex = 0;

        for (int i = 0; i < layer.Width; i ++)
        {
            for (int j = 0; j < layer.Height; j++)
            {
                // Read the GID in little-endian byte order:
                unsigned tileID = tileData[tileIndex] |
                                  tileData[tileIndex + 1] << 8  |
                                  tileData[tileIndex + 2] << 16 |
                                  tileData[tileIndex + 3] << 24;

                tileIndex += 4;

                bool flippedH = (tileID & FLIPPED_HORIZONTALLY_FLAG);
                bool flippedV = (tileID & FLIPPED_VERTICALLY_FLAG);
                bool flippedDiag = (tileID & FLIPPED_DIAGONALLY_FLAG);

                // Clear flags (Even the ones we are not using)
                tileID &= ~(FLIPPED_HORIZONTALLY_FLAG |
                            FLIPPED_VERTICALLY_FLAG |
                            FLIPPED_DIAGONALLY_FLAG |
                            ROTATED_HEXAGONAL_120_FLAG
                );

                Tile result = {};

                result.ID = (int)tileID;
                result.FlipHorizontal = flippedH;
                result.FlipVertical = flippedV;
                result.FlipDiagonal = flippedDiag;

                tiles.emplace_back(result);
            }
        }

        layer.Tiles = tiles;
        m_tileLayers.emplace_back(layer);
    }
}

void Tilemap::Draw(std::shared_ptr<Camera> &camera)
{
    // Draw all tiles in right-down order
    for (TileLayer layer : m_tileLayers)
    {
        for (int i = 0; i < layer.Width; i++)
        {
            for (int j = 0; j < layer.Height; j++)
            {
                int index = j * layer.Width + i;

                // 0 = no tile
                if (layer.Tiles[index].ID == 0)
                {
                    continue;
                }

                int tileID = layer.Tiles[index].ID;

                int tilesetIndex = 0;
                int tileIndex = 0;

                for (const Tileset& tileset : m_tileSets)
                {
                    if (tileset.FirstGID <= tileID &&
                        tileset.FirstGID + tileset.Length + 1 >= tileID)
                    {
                        tileIndex = tileID - tileset.FirstGID;
                        break;
                    }

                    tilesetIndex += 1;
                }

                const auto& tileSprite = m_tileSets[0].TileSprite;

                // Position
                tileSprite->SetPosition(glm::vec2(
                    m_tileFootprint * (float)i,
                    m_tileFootprint * (float)j
                ));

                // Set flipping
                tileSprite->SetFlipHorizontal(
                    layer.Tiles[index].FlipHorizontal
                );
                tileSprite->SetFlipVertical(
                    layer.Tiles[index].FlipVertical
                );
                tileSprite->SetFlipDiagonal(
                    layer.Tiles[index].FlipDiagonal
                );

                // Set size
                tileSprite->SetScale(glm::vec2(m_tileFootprint));

                // Draw!
                tileSprite->SetCurrentTileIndex(
                    tileIndex
                );

                tileSprite->Draw(camera);

                // Reset tile to identity transform
                tileSprite->ResetTransform();
            }
        }
    }
}
