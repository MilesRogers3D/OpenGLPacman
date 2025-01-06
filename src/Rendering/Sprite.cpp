#include "Sprite.h"

Sprite::Sprite(std::shared_ptr<Texture>& texture, glm::vec2 position, glm::vec2 size, glm::vec3 color)
    : Sprite(position, size, color)
{
    m_texture = texture;
    m_hasTexture = true;
}

Sprite::Sprite(glm::vec2 position, glm::vec2 size, glm::vec3 color)
{
    m_shader = ResourceManager::LoadShader(
        "res/shaders/sprite.vert",
        "res/shaders/sprite.frag",
        "SpriteShader"
    );

    m_position = position;
    m_size = size;
    m_color = color;

    InitRenderData();
}

Sprite::~Sprite()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

void Sprite::Draw()
{
    m_shader->Use();

    auto modelMatrix = glm::mat4(1.0F);

    // Translation
    modelMatrix = glm::translate(modelMatrix, glm::vec3(m_position, 0.0F));

    // Rotation (with respect to pivot point)
    glm::vec2 offset = m_pivot * m_size;

    modelMatrix = glm::translate(modelMatrix, glm::vec3(offset, 0.0F));
    modelMatrix = glm::rotate(modelMatrix, m_rotation, glm::vec3(0.0F, 0.0F, 1.0F));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-offset, 0.0F));

    // Sizing
    modelMatrix = glm::scale(modelMatrix, glm::vec3(m_size, 1.0F));

    // Shader setup
    m_shader->SetMat4("model", modelMatrix);
    m_shader->SetVec3("spriteColor", m_color);
    m_shader->SetMat4("projection", m_projMatrix);

    if (m_hasTexture)
    {
        m_shader->SetInt("image", 0);
        glActiveTexture(GL_TEXTURE0);
        m_texture->Bind();
    }

    // Bind VAO and draw geometry
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Sprite::InitRenderData()
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

void Sprite::SetRotation(float rotation)
{
    m_rotation = glm::radians(rotation);
}

void Sprite::AddRotation(float rotation)
{
    m_rotation += glm::radians(rotation);
}
