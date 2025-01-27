#include "Tilemap.h"

#include "Core/Log.h"
#include "Core/Scene/Components.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const unsigned FLIPPED_HORIZONTALLY_FLAG  = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG    = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG    = 0x20000000;
const unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000;

Tilemap::Tilemap(
    const std::shared_ptr<Scene>& scene,
    const char* path,
    const std::vector<TilemapInput>& tilemaps,
    int pixelsPerUnit)
{
    m_scene = scene;
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
            "Error loading tilemap! Non-square tilemaps not supported!"
        );
    }

    m_tileSize = tileHeight;

    // Set tile footprint in all sprites
    m_tileFootprint = (float)m_tileSize
        / (float)m_pixelsPerUnit * 100.0F;

    // Get first GIDs from each tile set
    if (data["tilesets"].size() != tilemaps.size())
    {
        Log::Critical(
            "Incorrect amount of TileSprites loaded in texture array!"
        );
    }

    // Get tilesets and match them to their TileSprites
    m_tileSets.reserve(tilemaps.size());
    int tilesetIndex = 0;

    for (json::iterator it = data["tilesets"].begin();
         it != data["tilesets"].end();
         it++)
    {
        Tileset tileset = {};

        tileset.FirstGID = it.value()["firstgid"];
        tileset.TilemapData = tilemaps[tilesetIndex];
        tileset.Length = tilemaps[tilesetIndex].Dimensions.x *
            tilemaps[tilesetIndex].Dimensions.y;
        
        m_tileSets.emplace_back(tileset);
        
        tilesetIndex++;
    }

    // Get layers
    m_tileLayers.clear();
    
    int layerID = 0;

    for (json::iterator it = data["layers"].begin();
         it != data["layers"].end();
         it++)
    {
        TileLayer layer = {};

        layer.Name = it.value()["name"];
        layer.Width = it.value()["width"];
        layer.Height = it.value()["height"];
        
        std::vector<uint32_t> entries = it.value()["data"];
        auto* tileData = reinterpret_cast<unsigned char*>(entries.data());
        
        std::vector<Entity> tileEntities;
        tileEntities.reserve(entries.size());

        for (int i = 0; i < layer.Width; i ++)
        {
            for (int j = 0; j < layer.Height; j++)
            {
                int tileIndex = (j * layer.Width + i) * 4;
                
                // Read the GID in little-endian byte order:
                int tileID = tileData[tileIndex] |
                             tileData[tileIndex + 1] << 8  |
                             tileData[tileIndex + 2] << 16 |
                             tileData[tileIndex + 3] << 24;

                bool flippedH = (tileID & FLIPPED_HORIZONTALLY_FLAG);
                bool flippedV = (tileID & FLIPPED_VERTICALLY_FLAG);
                bool flippedDiag = (tileID & FLIPPED_DIAGONALLY_FLAG);

                // Clear flags (Even the ones we are not using)
                tileID &= ~(FLIPPED_HORIZONTALLY_FLAG |
                            FLIPPED_VERTICALLY_FLAG |
                            FLIPPED_DIAGONALLY_FLAG |
                            ROTATED_HEXAGONAL_120_FLAG
                );
                
                if (tileID == 0)
                {
                    // No tile entity needs to be instantiated
                    // if its tileID = 0
                    continue;
                }
                
                // Find correct tilemap input
                int foundTilemap = (int)m_tileSets.size() - 1;
                int foundTile = m_tileSets[foundTilemap].Length - 1;
                
                for (int k = 0; k < m_tileSets.size() - 1; k++)
                {
                    if (m_tileSets[k].FirstGID <= tileID &&
                        m_tileSets[k + 1].FirstGID > tileID)
                    {
                        foundTile = (int)tileID - m_tileSets[k].FirstGID;
                        foundTilemap = k;
                        break;
                    }
                }
                
                // Create scene entity
                
                Entity result = m_scene.lock()->CreateEntity(
                    std::format(
                        "Layer{}Tile{}",
                        layerID,
                        j * layer.Width + i)
                );
                
                auto& transform = result.GetComponent<TransformComponent>();
                
                transform.Position = glm::vec2(
                    m_tileFootprint * (float)i,
                    m_tileFootprint * (float)j
                );
                transform.Size = glm::vec2(m_tileFootprint);
                
                auto& spriteRenderer = result.AddComponent<SpriteRendererComponent>();
                
                spriteRenderer.SetTexture(tilemaps[foundTilemap].Texture);
                spriteRenderer.FlipHorizontal = flippedH;
                spriteRenderer.FlipVertical = flippedV;
                spriteRenderer.FlipDiagonal = flippedDiag;
                
                auto& tileComponent = result.AddComponent<TileComponent>(
                    tilemaps[foundTilemap].Dimensions
                );
                
                tileComponent.TileIndex = foundTile;
                
                tileEntities.emplace_back(result);
            }
        }

        layer.TileEntities = tileEntities;
        m_tileLayers.emplace_back(layer);
        
        layerID += 1;
    }
}

void Tilemap::Draw(std::shared_ptr<Camera> &camera)
{
    // Draw all tiles in right-down order
    /*for (TileLayer layer : m_tileLayers)
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

                const auto& tileSprite = m_tileSets[tilesetIndex].TileSprite;

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
    }*/
}
