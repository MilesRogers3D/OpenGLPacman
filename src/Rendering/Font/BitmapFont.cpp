#include "BitmapFont.h"

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

void BitmapFont::LoadFont(
    std::shared_ptr<Texture>& texture,
    const char* jsonPath)
{
    // Load texture
    m_texture = texture;

    // Parse JSON file
    std::ifstream file(jsonPath);
    json data = json::parse(file)["characters"];

    for (json::iterator it = data.begin();
         it != data.end();
         it++)
    {
        auto charData = it.value();

        Character character = {};
        character.TextureId = m_texture->ID;
        character.Size = glm::ivec2(
            charData[0][0],
            charData[0][1]
        );
        character.Bearing = glm::ivec2(
            charData[1][0],
            charData[1][1]
        );
        character.Advance = charData[2];

        m_characters.insert(
            std::pair<char, Character>(it.key()[0], character)
        );
    }

    // Load shader
    m_textShader = ResourceManager::LoadShader(
        "res/shaders/font.vert",
        "res/shaders/font.frag",
        "TextShader"
    );

    // Setup buffers
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BitmapFont::RenderText(
    std::string text,
    glm::vec2 position,
    float size,
    glm::vec3 color,
    std::shared_ptr<Camera>& camera)
{
    m_textShader->Use();
    m_textShader->SetVec3("textColor", color);
    m_textShader->SetMat4("projection", camera->GetProjection());

    glActiveTexture(GL_TEXTURE0);
    m_texture->Bind();
    m_textShader->SetInt("text", 0);

    glBindVertexArray(m_VAO);

    std::string::const_iterator c;

    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_characters[*c];

        float w = ch.Size.x * size;
        float h = ch.Size.y * size;

        float u1 = (float)ch.Bearing.x / (float)m_texture->Width;
        float u2 = ((float)ch.Bearing.x + (float)ch.Size.x)
            / (float)m_texture->Width;
        float v1 = ((float)ch.Bearing.y + (float)ch.Size.y)
            / (float)m_texture->Height;
        float v2 = (float)ch.Bearing.y / (float)m_texture->Height;

        float vertices[6][4] =
        {
            { position.x,     position.y + h,   u1, v1 },
            { position.x,     position.y,       u1, v2 },
            { position.x + w, position.y,       u2, v2 },
    
            { position.x,     position.y + h,   u1, v1 },
            { position.x + w, position.y,       u2, v2 },
            { position.x + w, position.y + h,   u2, v1 }
        };

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        position.x += ((float)ch.Size.x + (float)ch.Advance) * size;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
