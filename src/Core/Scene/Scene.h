#pragma once

#include <entt/entt.hpp>
#include <string>

class Scene
{
    friend class Entity;
public:
    Scene();
    ~Scene();
    
    Entity CreateEntity(
        const std::string& name
    );
    
    entt::registry& GetRegistry();
    
private:
    entt::registry m_registry;
    std::string m_sceneName;
};