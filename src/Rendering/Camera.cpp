#include "Camera.h"
#include "IO/Log.h"

Camera::Camera(const glm::vec2& position, int width, int height)
    : m_position(position), m_width(width), m_height(height)
{
    RecalculateProjMatrix();

    Log::Info(
        "[Camera] Camera created at [%f, %f] with dimensions [%i, %i]",
        position.x,
        position.y,
        width,
        height
    );
}

void Camera::SetFrustumSize(int width, int height)
{
    m_width = width;
    m_height = height;

    RecalculateProjMatrix();
}

glm::mat4 Camera::GetProjection()
{
    return m_projMatrix;
}

void Camera::RecalculateProjMatrix()
{
    m_projMatrix = glm::ortho(
        0.0F + m_position.x,
        (float)m_width + m_position.x,
        (float)m_height + m_position.y,
        0.0F + m_position.y,
        -1.0F,
        1.0F
    );
}
