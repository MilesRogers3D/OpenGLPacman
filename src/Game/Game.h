#pragma once

#include "Core/Scene/Entity.h"
#include "Core/Systems/Renderer.h"
#include "Rendering/Sprite/Sprite.h"
#include "Rendering/Camera.h"
#include "Rendering/Font/BitmapFont.h"
#include "IO/Audio/AudioEmitter.h"
#include "IO/Tilemap/Tilemap.h"
#include "Game/Entities/Pacman.h"

class Game
{
    friend class Window;
protected:
    void Init();
    void Update(float deltaTime);
    void RenderGUI();
    void Render();
    void Destroy();

    // Engine events
    void OnKeyPressed(int key);
    void OnKeyReleased(int key);

    void OnWindowResize(int width, int height);

public:
    Game(Window* window);
    ~Game() = default;

private:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<BitmapFont> m_font;
    std::shared_ptr<AudioEmitter> m_audioEmitter;
    std::shared_ptr<Tilemap> m_tileMap;
    std::shared_ptr<Renderer> m_renderer;

    std::shared_ptr<Window> m_window;
    std::shared_ptr<Scene> m_scene;
    
    std::unique_ptr<Pacman> m_pacman;

    static std::map<std::string, std::shared_ptr<Sprite>> m_sprites;
    std::vector<Entity> m_entities;

private: // Settings
    int m_selectedEditorItem = 0;
    bool m_showCollision = true;
};
