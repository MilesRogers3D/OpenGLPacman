#pragma once

#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

#include <map>
#include <string>
#include <memory>

class ResourceManager
{
public:
    static std::map<std::string, std::shared_ptr<Shader>> Shaders;
    static std::map<std::string, std::shared_ptr<Texture>> Textures;

    static std::shared_ptr<Shader> LoadShader(
        const char* vertPath,
        const char* fragPath,
        const std::string& name
    );
    static std::shared_ptr<Shader> GetShader(const std::string& name);
    static bool HasShader(const std::string& name);

    static std::shared_ptr<Texture> LoadTexture(
        const char* path,
        const std::string& name
    );
    static std::shared_ptr<Texture> GetTexture(const std::string& name);

    static void DestroyAll();

private:
    ResourceManager();
};
