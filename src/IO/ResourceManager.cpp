#include "ResourceManager.h"

std::map<std::string, std::shared_ptr<Texture>> ResourceManager::Textures;
std::map<std::string, std::shared_ptr<Shader>> ResourceManager::Shaders;

std::shared_ptr<Shader> ResourceManager::LoadShader(
    const char* vertPath,
    const char* fragPath,
    const std::string& name)
{
    if (Shaders.contains(name))
    {
        return Shaders[name];
    }

    auto shader = std::make_shared<Shader>();
    shader->Load(vertPath, fragPath);

    Shaders[name] = shader;

    return Shaders[name];
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name)
{
    return Shaders[name];
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const char* path, const std::string& name)
{
    auto texture = std::make_shared<Texture>();
    texture->LoadTexture(path);

    Textures[name] = texture;

    return Textures[name];
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& name)
{
    return Textures[name];
}

void ResourceManager::DestroyAll()
{
    for (const auto& shader : Shaders)
    {
        glDeleteProgram(shader.second->ID);
    }
    for (const auto& texture : Textures)
    {
        glDeleteTextures(1, &texture.second->ID);
    }
}
