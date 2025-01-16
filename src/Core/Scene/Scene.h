#pragma once

#include <entt/entt.hpp>
#include <string>

class Scene
{
    friend class Entity;
public:
    Scene();
    ~Scene();

protected:
    entt::registry m_registry;
    std::string m_sceneName;
};