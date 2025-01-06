#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader
{
public:
    Shader() = default;

    unsigned int ID;

    Shader& Use();
    void Load(
        const char* vertPath,
        const char* fragPath
    );

    void SetFloat(
        const char* name,
        float value
    );
    void SetInt(
        const char* name,
        int value
    );
    void SetVec2(
        const char* name,
        const glm::vec2& value
    );
    void SetVec3(
        const char* name,
        const glm::vec3& value
    );
    void SetVec4(
        const char* name,
        const glm::vec4& value
    );
    void SetMat4(
        const char* name,
        const glm::mat4& value
    );

private:
    enum ShaderType
    {
        Vertex,
        Fragment,
        Program
    };

    std::string ReadTextFile(const char* path);
    void CheckCompilerErrors(unsigned int object, ShaderType type);
};
