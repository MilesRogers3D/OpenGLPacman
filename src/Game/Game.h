#pragma once

#include "Rendering/Sprite/Sprite.h"
#include "Rendering/Camera.h"
#include "Rendering/Font/BitmapFont.h"
#include "IO/Audio/AudioEmitter.h"
#include "IO/Tilemap/Tilemap.h"

class Game
{
    friend class Window;
protected:
    void Init();
    void Update(float deltaTime);
    void Render();
    void Destroy();

    // Engine events
    void OnKeyPressed(int key);
    void OnKeyReleased(int key);

    void OnWindowResize(int width, int height);

public:
    Game() = default;
    ~Game() = default;

private:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<BitmapFont> m_font;
    std::shared_ptr<AudioEmitter> m_audioEmitter;
    std::shared_ptr<Tilemap> m_tileMap;

    static std::map<std::string, std::shared_ptr<Sprite>> m_sprites;
};
