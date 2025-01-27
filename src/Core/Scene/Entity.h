#pragma once

#include "Scene.h"
#include "Core/Log.h"

#include <entt/entt.hpp>

class Entity
{
public:
    Entity() = default;
    Entity(
        entt::entity handle,
        const std::shared_ptr<Scene>& scene
    );
    Entity(const Entity& other) = default;
    
    ~Entity() = default;

    template<typename T, typename ... Args>
    T& AddComponent(Args&& ... args)
    {
        T& component = m_scene->GetRegistry().emplace<T>(
            m_handle,
            std::forward<Args>(args) ...
        );
        
        return component;
    }

    template<typename T>
    T& GetComponent()
    {
        if (!HasComponent<T>())
        {
            Log::Warning(
                "[Scene] Entity does not have component '%s'!",
                typeid(T).name()
            );
        }
        
        return m_scene->GetRegistry().get<T>(m_handle);
    }
    
    template<typename T>
    bool HasComponent()
    {
        return m_scene->GetRegistry().any_of<T>(m_handle);
    }
    
    void OnGUIDraw();

private:
    entt::entity m_handle = entt::null;
    std::shared_ptr<Scene> m_scene;
};
