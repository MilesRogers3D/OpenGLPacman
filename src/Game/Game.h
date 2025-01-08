#pragma once

#include "Rendering/Sprite.h"
#include "Rendering/Camera.h"

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

public:
    Game();
    ~Game() = default;

private:
    std::shared_ptr<Camera> m_camera;

    static std::map<std::string, std::shared_ptr<Sprite>> m_sprites;
};
