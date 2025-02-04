#include "Sprite.h"

#include "Rendering/Debug/DebugShapes.h"

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

void Sprite::Draw(std::shared_ptr<Camera>& camera)
{
    m_shader->Use();

    // (Matrix transformations are applied in reverse order)
    auto modelMatrix = glm::mat4(1.0F);
    glm::vec2 offset = m_pivot * m_size;

    // Translation
    modelMatrix = glm::translate(modelMatrix, glm::vec3(m_position, 0.0F));

    // Reset pivot
    modelMatrix = glm::translate(modelMatrix, glm::vec3(offset, 0.0F));

    // Rotation
    modelMatrix = glm::rotate(modelMatrix, m_rotation, glm::vec3(0.0F, 0.0F, 1.0F));

    // Flipping
    if (m_flipHorizontal)
    {
        modelMatrix = glm::rotate(
            modelMatrix,
            glm::radians(180.0F),
            glm::vec3(0.0F, 1.0F, 0.0F)
        );
    }
    if (m_flipVertical)
    {
        modelMatrix = glm::rotate(
            modelMatrix,
            glm::radians(180.0F),
            glm::vec3(1.0F, 0.0F, 0.0F)
        );
    }
    if (m_flipDiagonal)
    {
        modelMatrix = glm::rotate(
            modelMatrix,
            glm::radians(180.0F),
            glm::vec3(0.71F, 0.71F, 0.0F)
        );
    }

    // Set pivot point
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-offset, 0.0F));

    // Sizing
    modelMatrix = glm::scale(modelMatrix, glm::vec3(m_size, 1.0F));

    // Shader setup
    m_shader->SetMat4("model", modelMatrix);
    m_shader->SetVec3("spriteColor", m_color);
    m_shader->SetMat4("projection", camera->GetProjection());

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

void Sprite::DrawCollision(std::shared_ptr<Camera>& camera)
{
    if (!m_drawCollision)
    {
        return;
    }

    ColliderData colData = GetColliderData();

    std::vector<glm::vec2> boxPoints;
    boxPoints.reserve(4);

    boxPoints.emplace_back(
        colData.WorldOrigin.x - colData.WorldSize.x * 0.5F,
        colData.WorldOrigin.y - colData.WorldSize.y * 0.5F
    );
    boxPoints.emplace_back(
        colData.WorldOrigin.x - colData.WorldSize.x * 0.5F,
        colData.WorldOrigin.y + colData.WorldSize.y * 0.5F
    );
    boxPoints.emplace_back(
        colData.WorldOrigin.x + colData.WorldSize.x * 0.5F,
        colData.WorldOrigin.y + colData.WorldSize.y * 0.5F
    );
    boxPoints.emplace_back(
        colData.WorldOrigin.x + colData.WorldSize.x * 0.5F,
        colData.WorldOrigin.y - colData.WorldSize.y * 0.5F
    );
    boxPoints.emplace_back(
        colData.WorldOrigin.x - colData.WorldSize.x * 0.5F,
        colData.WorldOrigin.y - colData.WorldSize.y * 0.5F
    );

    DebugShapes::DrawLine(boxPoints, camera);
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

void Sprite::Update(float deltaTime)
{

}

void Sprite::SetPosition(glm::vec2 position)
{
    m_position = position;
}

void Sprite::AddPosition(glm::vec2 position)
{
    m_position += position;
}

void Sprite::SetScale(glm::vec2 scale)
{
    m_size = scale;
}

void Sprite::SetFlipHorizontal(bool flip)
{
    m_flipHorizontal = flip;
}

void Sprite::SetFlipVertical(bool flip)
{
    m_flipVertical = flip;
}

void Sprite::SetFlipDiagonal(bool flip)
{
    m_flipDiagonal = flip;
}

void Sprite::ResetTransform()
{
    m_position = glm::vec2(0.0F);
    m_rotation = 0.0F;
    m_size = glm::vec2(1.0F);

    m_flipHorizontal = false;
    m_flipVertical = false;
    m_flipDiagonal = false;
}

void Sprite::SetCollisionEnabled(bool enable)
{
    m_hasCollision = true;
}

void Sprite::SetDrawCollision(bool enable)
{
#ifdef _DEBUG
    m_drawCollision = true;
#endif
}

Sprite::ColliderData Sprite::GetColliderData()
{
    ColliderData data = {};

    data.WorldOrigin = m_position + m_size * m_pivot
        + m_boxCollider.Origin;
    data.WorldSize = m_size * m_boxCollider.Size;

    return data;
}
