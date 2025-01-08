#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera(const glm::vec2& position, int width, int height);
    ~Camera() = default;

    void SetFrustumSize(int width, int height);
    glm::mat4 GetProjection();

private:
    glm::vec2 m_position = glm::vec2(0.0F);
    int m_width = 0;
    int m_height = 0;

    void RecalculateProjMatrix();
    glm::mat4 m_projMatrix = glm::mat4(1.0F);
};
