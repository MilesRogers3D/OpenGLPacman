#pragma once

#include "IO/ResourceManager.h"
#include "Rendering/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <memory>
#include <utility>

struct Character
{
    unsigned int TextureId;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class BitmapFont
{
    friend class Renderer;
public:
    void LoadFont(
        std::shared_ptr<Texture>& texture,
        const char* jsonPath
    );

    void RenderText(
        std::string text,
        glm::vec2 position,
        float size,
        glm::vec3 color,
        std::shared_ptr<Camera>& camera
    );
    
    void BindTexture();

    BitmapFont() = default;
    ~BitmapFont() = default;
    
protected:
    Character GetCharacter(char character);
    
    [[nodiscard]]
    const std::shared_ptr<Texture>& GetTexture() const;
    
    [[nodiscard]]
    unsigned int GetVAO() const;
    [[nodiscard]]
    unsigned int GetVBO() const;
    
private:
    std::map<char, Character> m_characters;

    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<Shader> m_textShader;

    unsigned int m_VAO, m_VBO;
};
