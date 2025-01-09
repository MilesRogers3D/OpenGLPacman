#include "Game.h"

#include "IO/ResourceManager.h"
#include "IO/Log.h"
#include "Entities/Pacman.h"
#include "Rendering/Font/BitmapFont.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

std::map<std::string, std::shared_ptr<Sprite>> Game::m_sprites;

#define ROTATE_SPEED 15.0F

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
        }
    }

    // Maze setup
    auto mazeTex = ResourceManager::LoadTexture("res/sprites/maze.png", "Maze");

    m_sprites["Maze"] = std::make_shared<Sprite>(
        mazeTex,
        glm::vec2(0.0F, 96.0F),
        glm::vec2(896.0F, 992.0F),
        glm::vec3(1.0F)
    );

    // Pacman animated sprite setup
    auto pacmanTex = ResourceManager::LoadTexture("res/sprites/pacman.png", "Pacman");

    m_sprites["Pacman"] = std::make_shared<Pacman>(
        3,
        0.25F,
        pacmanTex,
        glm::vec2(500.0F, 500.0F),
        glm::vec2(52.0F),
        glm::vec3(1.0F)
    );

    // Play intro sound
    m_audioEmitter->Play("res/audio/music/intro.wav");
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
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto sprite : m_sprites)
    {
        sprite.second->Draw(m_camera);
    }

    m_font->RenderText(
        "HELLO PAC-MAN! - # 500 @$",
        glm::vec2(10.0F),
        3.0F,
        glm::vec3(1.0F),
        m_camera
    );
}

void Game::Destroy()
{
    Log::Info("Shutting down...");
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
        if (Pacman* character = dynamic_cast<Pacman*>(sprite.second.get()))
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
