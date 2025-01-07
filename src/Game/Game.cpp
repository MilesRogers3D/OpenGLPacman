#include "Game.h"

#include "IO/ResourceManager.h"
#include "IO/Log.h"
#include "Entities/PlayableCharacter.h"
#include <glad/glad.h>
#include <iostream>

std::map<std::string, std::shared_ptr<Sprite>> Game::m_sprites;

#define ROTATE_SPEED 15.0F

void Game::Init()
{
    Log::Trace("Hello game!");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
                glm::vec2(64.0F),
                glm::vec3(1.0F)
            );
        }
    }

    // Pacman animated sprite setup
    auto pacmanTex = ResourceManager::LoadTexture("res/sprites/pacman.png", "Pacman");

    m_sprites["Pacman"] = std::make_shared<PlayableCharacter>(
        3,
        0.25F,
        pacmanTex,
        glm::vec2(500.0F, 500.0F),
        glm::vec2(64.0F),
        glm::vec3(1.0F)
    );
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
        sprite.second->Draw();
    }
}

void Game::Destroy()
{
    Log::Info("Shutting down...");
    ResourceManager::DestroyAll();
}

void Game::OnKeyPressed(int key)
{
    for (auto sprite : m_sprites)
    {
        if (PlayableCharacter* character =
            dynamic_cast<PlayableCharacter*>(sprite.second.get()))
        {
            character->OnKeyPressed(key);
        }
    }

    Log::Info("Key pressed: %i", key);
}

void Game::OnKeyReleased(int key)
{
    for (auto sprite : m_sprites)
    {
        if (PlayableCharacter* character =
            dynamic_cast<PlayableCharacter*>(sprite.second.get()))
        {
            character->OnKeyReleased(key);
        }
    }

    Log::Info("Key released: %i", key);
}
