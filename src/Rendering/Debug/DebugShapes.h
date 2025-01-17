#pragma once

#include "Rendering/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

class DebugShapes
{
public:
    static void DrawLine(
        const std::vector<glm::vec2>& vertices,
        const std::shared_ptr<Camera>& camera
    );
};
