#pragma once

#include "Scene.h"
#include "Core/Log.h"

#include <entt/entt.hpp>

class Entity
{
public:
    explicit Entity(
        const std::shared_ptr<Scene>& scene,
        const std::string& name
    );

    template<typename T>
    bool HasComponent()
    {
        return m_scene->m_registry.any_of<T>(m_handle);
    }

    template<typename T, typename ... Args>
    T& AddComponent(Args&& ... args)
    {
        return m_scene->m_registry.emplace<T>(
            m_handle,
            std::forward<Args>(args) ...
        );
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

            return nullptr;
        }

        return m_scene->m_registry.get<T>(m_handle);
    }


private:
    entt::entity m_handle;
    std::shared_ptr<Scene> m_scene;
};
