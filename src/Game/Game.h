#pragma once

#include "Rendering/Sprite.h"

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
    Game() = default;
    ~Game() = default;

private:
    static std::map<std::string, std::shared_ptr<Sprite>> m_sprites;
};
