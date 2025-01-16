#include "Entity.h"
#include "Components.h"

Entity::Entity(
    const std::shared_ptr<Scene>& scene,
    const std::string& name)
        : m_scene(scene)
{
    m_handle = scene->m_registry.create();

    AddComponent<NameComponent>(name);
    AddComponent<TagComponent>();
    AddComponent<TransformComponent>();
}
