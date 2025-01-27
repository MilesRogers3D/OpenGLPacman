#include "Renderer.h"
#include "Core/Scene/Components.h"
#include "IO/ResourceManager.h"
#include "Rendering/Debug/DebugShapes.h"
#include "Core/Log.h"

#include <entt/entt.hpp>

Renderer::Renderer(const std::shared_ptr<Scene> &scene)
{
    m_scene = scene;
    
    SetupRenderQuad();
    
    m_spriteShader = ResourceManager::LoadShader(
        "res/shaders/sprite.vert",
        "res/shaders/sprite.frag",
        "SpriteShader"
    );
}

Renderer::~Renderer()
{
    DestroyRenderQuad();
}

void Renderer::RenderSprites(const std::shared_ptr<Camera>& camera)
{
    auto shader = m_spriteShader.lock();
    
    // Render every component in scene with the required data
    auto scene = m_scene.lock();
    
    auto view = scene->GetRegistry().view<
        const TransformComponent,
        const SpriteRendererComponent
    >();
    
    for (const auto& [entity, transform, spriteRenderer] : view.each())
    {
        auto modelMatrix = glm::mat4(1.0F);
        
        glm::vec2 offset = transform.Pivot * transform.Size;
        
        // Translation
        modelMatrix = glm::translate(
            modelMatrix,
            glm::vec3(
                transform.Position,
                0.0F
            ));
        
        // Reset pivot
        modelMatrix = glm::translate(
            modelMatrix,
            glm::vec3(
                offset,
                0.0F
            ));
        
        // Rotation
        modelMatrix = glm::rotate(
            modelMatrix,
            transform.Rotation,
            glm::vec3(0.0F, 0.0F, 1.0F)
        );
        
        // Flipping
        if (spriteRenderer.FlipHorizontal)
        {
            modelMatrix = glm::rotate(
                modelMatrix,
                glm::radians(180.0F),
                glm::vec3(0.0F, 1.0F, 0.0F)
            );
        }
        
        if (spriteRenderer.FlipVertical)
        {
            modelMatrix = glm::rotate(
                modelMatrix,
                glm::radians(180.0F),
                glm::vec3(1.0F, 0.0F, 0.0F)
            );
        }
        
        if (spriteRenderer.FlipDiagonal)
        {
            modelMatrix = glm::rotate(
                modelMatrix,
                glm::radians(180.0F),
                glm::vec3(0.71F, 0.71F, 0.0F)
            );
        }
        
        // Set pivot point
        modelMatrix = glm::translate(
            modelMatrix,
            glm::vec3(-offset, 0.0F)
        );
        
        // Sizing
        modelMatrix = glm::scale(
            modelMatrix,
            glm::vec3(transform.Size, 1.0F)
        );
        
        shader->Use();
        
        if (scene->GetRegistry().all_of<
            FlipbookComponent,
            TileComponent>(entity))
        {
            Log::Critical(
                "Entity '%s' cannot have both a FlipbookComponent"
                "and a TileComponent!",
                scene->GetRegistry().get<NameComponent>(
                    entity).Name.c_str()
            );
        }
        
        // Set animation frame (if needed)
        if (scene->GetRegistry().any_of<FlipbookComponent>(entity))
        {
            auto& flipbook = scene->GetRegistry()
                .get<FlipbookComponent>(entity);
            
            int frame = (int)(
                flipbook.GetCurrentTime() /
                flipbook.FrameDuration) %
                flipbook.Divisions;
            
            shader->SetInt("xDivisions", flipbook.Divisions);
            shader->SetInt("frame", frame);
        }
        else if (scene->GetRegistry().any_of<TileComponent>(entity))
        {
            auto& tile = scene->GetRegistry()
                .get<TileComponent>(entity);
            
            shader->SetInt("xDivisions", tile.Divisions.x);
            shader->SetInt("yDivisions", tile.Divisions.y);
            shader->SetInt("frame", tile.TileIndex);
        }
        else
        {
            shader->SetInt("xDivisions", 1);
            shader->SetInt("yDivisions", 1);
            shader->SetInt("frame", 0);
        }
        
        // Shader setup
        shader->SetMat4("model", modelMatrix);
        shader->SetVec3("spriteColor", spriteRenderer.ColorTint);
        shader->SetMat4("projection", camera->GetProjection());
        
        if (spriteRenderer.HasTexture())
        {
            shader->SetInt("image", 0);
            glActiveTexture(GL_TEXTURE0);
            
            auto texture = spriteRenderer.GetTexture().lock();
            texture->Bind();
        }
        
        // Bind VAO and draw geometry
        glBindVertexArray(m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Un-bind VAO
        glBindVertexArray(0);
        
        // Draw collision if needed
        if (scene->GetRegistry().any_of<BoxColliderComponent>(entity))
        {
            auto& collider = scene->GetRegistry()
                .get<BoxColliderComponent>(entity);
            
            if (collider.DrawDebugCollision)
            {
                glm::vec2 worldOrigin = transform.Position +
                    transform.Size * transform.Pivot +
                    collider.Position;
                glm::vec2 worldSize = transform.Size *
                    collider.Size;
                
                std::vector<glm::vec2> boxPoints;
                boxPoints.reserve(5);
                
                boxPoints.emplace_back(
                    worldOrigin.x - worldSize.x * 0.5F,
                    worldOrigin.y - worldSize.y * 0.5F
                );
                boxPoints.emplace_back(
                    worldOrigin.x - worldSize.x * 0.5F,
                    worldOrigin.y + worldSize.y * 0.5F
                );
                boxPoints.emplace_back(
                    worldOrigin.x + worldSize.x * 0.5F,
                    worldOrigin.y + worldSize.y * 0.5F
                );
                boxPoints.emplace_back(
                    worldOrigin.x + worldSize.x * 0.5F,
                    worldOrigin.y - worldSize.y * 0.5F
                );
                boxPoints.emplace_back(
                    worldOrigin.x - worldSize.x * 0.5F,
                    worldOrigin.y - worldSize.y * 0.5F
                );
                
                DebugShapes::DrawLine(boxPoints, camera);
            }
        }
    }
}

void Renderer::SetupRenderQuad()
{
    unsigned int VBO;
    float vertices[] =
    {
        // Pos      // UV
        0.0F, 1.0F, 0.0F, 1.0F,
        1.0F, 0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 0.0F,
        
        0.0F, 1.0F, 0.0F, 1.0F,
        1.0F, 1.0F, 1.0F, 1.0F,
        1.0F, 0.0F, 1.0F, 0.0F
    };
    
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &VBO);
    
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );
    
    glBindVertexArray(m_quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        4,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)nullptr
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::DestroyRenderQuad()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}
