#pragma once

#include "Core/Scene/Scene.h"
#include "Rendering/Camera.h"
#include "Rendering/Shader.h"

#include <memory>

class Renderer
{
public:
    Renderer(const std::shared_ptr<Scene>& scene);
    ~Renderer();

    void RenderSprites(const std::shared_ptr<Camera>& camera);

private:
    void SetupRenderQuad();
    void DestroyRenderQuad();
    
    unsigned int m_quadVAO;
    
    std::weak_ptr<Shader> m_spriteShader;
    std::weak_ptr<Scene> m_scene;
};
