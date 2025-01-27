#include "Game.h"

#include "Core/Window.h"
#include "IO/ResourceManager.h"
#include "Core/Log.h"
#include "Entities/Pacman.h"
#include "Rendering/Font/BitmapFont.h"
#include "Rendering/Debug/DebugShapes.h"
#include "Core/Scene/Entity.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Components.h"
#include "IO/Tilemap/Tilemap.h"

#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

std::map<std::string, std::shared_ptr<Sprite>> Game::m_sprites;

constexpr bool muteGame = true;

#define ROTATE_SPEED 15.0F

Game::Game(Window* window)
{
    std::shared_ptr<Window> windowPtr(window);
    m_window = windowPtr;
    
    m_scene = std::make_shared<Scene>();
}

void Game::Init()
{
    Log::Trace("Hello game!");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Camera setup
    m_camera = std::make_shared<Camera>(
        glm::vec2(0.0F),
        896,
        1152
    );

    // Audio emitter setup
    m_audioEmitter = std::make_shared<AudioEmitter>();

    auto fontTex = ResourceManager::LoadTexture(
        "res/fonts/font.png",
        "FontTexture"
    );
    m_font = std::make_shared<BitmapFont>();
    m_font->LoadFont(
        fontTex,
        "res/fonts/font.json"
    );
    
    // Renderer setup
    m_renderer = std::make_shared<Renderer>(m_scene);

    // Ghost sprite setup
    auto ghostTex = ResourceManager::LoadTexture("res/sprites/ghost.png", "Blinky");

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            std::string name = "Blinky" + std::to_string(5 * i + j);
            glm::vec2 pos = glm::vec2(100.0F) + (glm::vec2((float)i, (float)j) * 50.0F);
            
            Log::Info("Creating sprite %s at [%f, %f]",
                name.c_str(),
                pos.x,
                pos.y
            );
            
            Entity ghostEntity = m_scene->CreateEntity(name);
            
            SpriteRendererComponent srCompData = {};
            
            srCompData.SetTexture(ghostTex);
            srCompData.ColorTint = glm::vec4(1.0F);
            
            ghostEntity.AddComponent<SpriteRendererComponent>(ghostTex, glm::vec4(1.0F));
            ghostEntity.AddComponent<BoxColliderComponent>().DrawDebugCollision = true;
            
            auto& t = ghostEntity.GetComponent<TransformComponent>();
            
            t.Position = glm::vec2(100.0F) + (glm::vec2((float)i, (float)j) * 50.0F);
            t.Size = glm::vec2(56.0F);
            
            m_entities.emplace_back(ghostEntity);
        }
    }

    // Maze setup
    std::vector<TilemapInput> tilemaps;
    tilemaps.reserve(3);

    auto mazeTex = ResourceManager::LoadTexture("res/sprites/maze_tileset.png", "MazeTileset");
    auto dotTex = ResourceManager::LoadTexture("res/sprites/dots.png", "MazeTileset");
    auto debugTex = ResourceManager::LoadTexture("res/sprites/maze_tileset.png", "MazeTileset");

    TilemapInput mazeTilemap = {};
    mazeTilemap.Dimensions = glm::ivec2(6, 2);
    mazeTilemap.Texture = mazeTex;
    
    TilemapInput dotTilemap = {};
    dotTilemap.Dimensions = glm::ivec2(2, 1);
    dotTilemap.Texture = dotTex;
    
    TilemapInput debugTilemap = {};
    debugTilemap.Dimensions = glm::ivec2(1, 1);
    debugTilemap.Texture = debugTex;
    
    tilemaps.emplace_back(mazeTilemap);
    tilemaps.emplace_back(dotTilemap);
    tilemaps.emplace_back(debugTilemap);

    m_tileMap = std::make_shared<Tilemap>(
        m_scene,
        "res/maps/level.json",
        tilemaps,
        25
    );

    // Pacman animated sprite setup
    auto pacmanTex = ResourceManager::LoadTexture("res/sprites/pacman.png", "Pacman");

    Entity pacman = m_scene->CreateEntity("Pacman");
    
    pacman.AddComponent<SpriteRendererComponent>(pacmanTex, glm::vec4(1.0F));
    pacman.AddComponent<FlipbookComponent>(3, 0.1F);
    pacman.AddComponent<BoxColliderComponent>().DrawDebugCollision = true;
    
    auto& transform = pacman.GetComponent<TransformComponent>();
    
    transform.Position = glm::vec2(425.0F, 630.0F);
    transform.Size = glm::vec2(52.0F);
    
    m_entities.emplace_back(pacman);

    // Play intro sound
    if (!muteGame)
    {
        m_audioEmitter->Play("res/audio/music/intro.wav");
    }
}

void Game::Update(float deltaTime)
{
    for (auto sprite : m_sprites)
    {
        sprite.second->Update(deltaTime);
    }
}

void Game::Render()
{
    // Game rendering
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_renderer->RenderSprites(m_camera);
}

void Game::RenderGUI()
{
    // Debug flags window
    if (ImGui::Begin("Debug Console"))
    {
        ImGui::SeparatorText("Debug Flags:");
        ImGui::Checkbox("Show Collision", &m_showCollision);
    }
    
    ImGui::End();
    
    // Entity list window
    int selectedItem = 0;
    bool itemFound = false;
    
    if (ImGui::Begin("Entity List"))
    {
        m_scene->GetRegistry().sort<NameComponent>([](
            const NameComponent& lhs,
            const NameComponent& rhs)
        {
            return lhs.Name < rhs.Name;
        });
        
        auto entities = m_scene->GetRegistry().view<NameComponent>();
        
        for (auto entity : entities)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
            
            if (selectedItem == m_selectedEditorItem)
            {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            
            if (ImGui::TreeNodeEx(
                m_scene->GetRegistry().get<NameComponent>(entity).Name.c_str(),
                flags))
            {
                if (ImGui::IsItemClicked())
                {
                    itemFound = true;
                    m_selectedEditorItem = selectedItem;
                }
                
                if (!itemFound)
                {
                    selectedItem += 1;
                }
                
                ImGui::TreePop();
            }
        }
    }
    
    ImGui::End();
    
    // Inspector window
    if (ImGui::Begin("Inspector"))
    {
        if (itemFound)
        {
            ImGui::SeparatorText(
                m_entities[m_selectedEditorItem].
                    GetComponent<NameComponent>().Name.c_str()
            );
            m_entities[m_selectedEditorItem].OnGUIDraw();
        }
    }
    
    ImGui::End();
}

void Game::Destroy()
{
    Log::Info("Shutting down...");

    // Game shutdown
    ResourceManager::DestroyAll();
}

void Game::OnKeyPressed(int key)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        // TODO: Add pausing
        exit(1);
    }

    for (auto sprite : m_sprites)
    {
        if (Pacman* character = dynamic_cast<Pacman*>(
            sprite.second.get()))
        {
            character->OnKeyPressed(key);
        }
    }

    m_audioEmitter->Play("res/audio/sfx/chomp.wav");

    Log::Info("Key pressed: %i", key);
}

void Game::OnKeyReleased(int key)
{
    for (auto sprite : m_sprites)
    {
        if (Pacman* character =
            dynamic_cast<Pacman*>(sprite.second.get()))
        {
            character->OnKeyReleased(key);
        }
    }

    Log::Info("Key released: %i", key);
}

void Game::OnWindowResize(int width, int height)
{
    m_camera->SetFrustumSize(width, height);
}
