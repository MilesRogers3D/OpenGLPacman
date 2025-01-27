#include "Scene.h"
#include "Core/Log.h"
#include "Entity.h"
#include "Components.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

Entity Scene::CreateEntity(const std::string& name)
{
    std::shared_ptr<Scene> scenePtr(this);
    
    Entity entity =
    {
        m_registry.create(),
        scenePtr
    };
    
    entity.AddComponent<NameComponent>(name);
    entity.AddComponent<TransformComponent>();
    
    return entity;
}

entt::registry& Scene::GetRegistry()
{
    return m_registry;
}