#include "DebugShapes.h"
#include "IO/ResourceManager.h"

#include <array>

void DebugShapes::DrawLine(
    const std::vector<glm::vec2> &vertices,
    const std::shared_ptr<Camera> &camera)
{
    // Load shader from ResourceManager
    if (!ResourceManager::HasShader("Debug"))
    {
        ResourceManager::LoadShader(
            "res/shaders/debug/solid_color.vert",
            "res/shaders/debug/solid_color.frag",
            "Debug"
        );
    }

    auto shader = ResourceManager::GetShader("Debug");

    // Decompose input data
    std::vector<float> vertData;
    vertData.reserve(vertices.size());

    for (glm::vec2 vert : vertices)
    {
        vertData.emplace_back(vert.x);
        vertData.emplace_back(vert.y);
    }

    // Construct buffers
    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertData.size() * sizeof(float),
        vertData.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)nullptr
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Drawing
    shader->Use();
    shader->SetMat4("projection", camera->GetProjection());
    shader->SetVec3("color", glm::vec3(0.0F, 1.0F, 0.0F));

    glDrawArrays(GL_LINE_STRIP, 0, vertData.size() / 2);
    glBindVertexArray(0);
}
