#include "Renderer.h"
#include "Core/Scene/Components.h"
#include "IO/ResourceManager.h"
#include "Rendering/Debug/DebugShapes.h"
#include "Rendering/Font/BitmapFont.h"
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
    
    m_fontShader = ResourceManager::LoadShader(
        "res/shaders/font.vert",
        "res/shaders/font.frag",
        "FontShader"
    );
}

Renderer::~Renderer()
{
    DestroyRenderQuad();
}

void Renderer::RenderSprites(const std::shared_ptr<Camera>& camera)
{
    auto shader = m_spriteShader.lock();
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    
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

void Renderer::RenderFonts(const std::shared_ptr<Camera> &camera)
{
    auto scene = m_scene.lock();
    auto shader = m_fontShader.lock();
    
    shader->Use();
    shader->SetMat4("projection", camera->GetProjection());
    
    auto view = scene->GetRegistry().view<
        const TransformComponent,
        const FontRendererComponent
    >();
    
    for (const auto& [entity, transform, fontRenderer] : view.each())
    {
        shader->SetVec3("textColor", fontRenderer.Color);
        
        glActiveTexture(GL_TEXTURE0);
        fontRenderer.Font.lock()->BindTexture();
        
        shader->SetInt("text", 0);
        
        auto bitmapFont = fontRenderer.Font.lock();
        glm::vec2 position = transform.Position;
        
        glBindVertexArray(bitmapFont->GetVAO());
        
        std::string text = fontRenderer.Text;
        
        // Make uppercase so it can be read by the font parser
        std::transform(text.begin(), text.end(), text.begin(), ::toupper);
        
        for (char& c : text)
        {
            Character ch = bitmapFont->GetCharacter(c);
            
            float w = (float)ch.Size.x * fontRenderer.FontSize;
            float h = (float)ch.Size.y * fontRenderer.FontSize;
            
            auto texture = bitmapFont->GetTexture();
            
            float u1 =   (float)ch.Bearing.x / (float)texture->Width;
            float u2 =  ((float)ch.Bearing.x + (float)ch.Size.x)
                       / (float)texture->Width;
            float v1 =  ((float)ch.Bearing.y + (float)ch.Size.y)
                       / (float)texture->Height;
            float v2 =   (float)ch.Bearing.y / (float)texture->Height;
            
            float vertices[6][4] =
            {
                { position.x,     position.y + h,   u1, v1 },
                { position.x,     position.y,       u1, v2 },
                { position.x + w, position.y,       u2, v2 },
                
                { position.x,     position.y + h,   u1, v1 },
                { position.x + w, position.y,       u2, v2 },
                { position.x + w, position.y + h,   u2, v1 }
            };
            
            glBindBuffer(GL_ARRAY_BUFFER, bitmapFont->GetVBO());
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            position.x += ((float)ch.Size.x + (float)ch.Advance) *
                fontRenderer.FontSize;
        }
        
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Renderer::SetupRenderQuad()
{
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
    glGenBuffers(1, &m_quadVBO);
    
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
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
