#include "Game.h"

#include "Core/Window.h"
#include "IO/ResourceManager.h"
#include "Core/Log.h"
#include "Entities/Pacman.h"
#include "Rendering/Font/BitmapFont.h"
#include "Rendering/Debug/DebugShapes.h"

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

            m_sprites[name] = std::make_shared<Sprite>(
                ghostTex,
                pos,
                glm::vec2(56.0F),
                glm::vec3(1.0F)
            );
            m_sprites[name]->SetCollisionEnabled(true);
            m_sprites[name]->SetDrawCollision(true);
        }
    }

    // Maze setup
    std::vector<std::shared_ptr<TileSprite>> tileSprites;
    tileSprites.clear();

    auto mazeTex = ResourceManager::LoadTexture("res/sprites/maze_tileset.png", "MazeTileset");
    auto dotTex = ResourceManager::LoadTexture("res/sprites/dots.png", "MazeTileset");
    auto debugTex = ResourceManager::LoadTexture("res/sprites/maze_tileset.png", "MazeTileset");

    tileSprites.emplace_back(std::make_shared<TileSprite>(
        glm::ivec2(6, 2),
        mazeTex
    ));
    tileSprites.emplace_back(std::make_shared<TileSprite>(
        glm::ivec2(2, 1),
        dotTex
    ));
    tileSprites.emplace_back(std::make_shared<TileSprite>(
        glm::ivec2(1, 1),
        debugTex
    ));

    m_tileMap = std::make_shared<Tilemap>(
        "res/maps/level.json",
        tileSprites,
        25
    );

    // Pacman animated sprite setup
    auto pacmanTex = ResourceManager::LoadTexture("res/sprites/pacman.png", "Pacman");

    m_sprites["Pacman"] = std::make_shared<Pacman>(
        3,
        0.1F,
        pacmanTex,
        glm::vec2(425.0F, 630.0F),
        glm::vec2(52.0F),
        glm::vec3(1.0F)
    );
    m_sprites["Pacman"]->SetCollisionEnabled(true);
    m_sprites["Pacman"]->SetDrawCollision(true);

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

    m_tileMap->Draw(m_camera);

    for (auto sprite : m_sprites)
    {
        sprite.second->Draw(m_camera);

        if (m_showCollision)
        {
            sprite.second->DrawCollision(m_camera);
        }
    }

    m_font->RenderText(
        "HELLO PAC-MAN! - # 500 @$",
        glm::vec2(10.0F),
        3.0F,
        glm::vec3(1.0F),
        m_camera
    );

    m_font->RenderText(
        m_showCollision ? "COLLISION VIEW ENABLED" : "COLLISION VIEW DISABLED",
        glm::vec2(10.0F, 45.0F),
        1.0F,
        glm::vec3(1.0F),
        m_camera
    );
}

void Game::RenderGUI()
{
    // Inspector window
    if (ImGui::Begin("Debug Console"))
    {
        ImGui::SeparatorText("Debug Flags:");
        ImGui::Checkbox("Show Collision", &m_showCollision);
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
