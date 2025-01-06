#include "Shader.h"
#include "IO/Log.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

Shader& Shader::Use()
{
    glUseProgram(ID);
    return *this;
}

// Note: No manual error-checking in favor of OpenGL 4.3's
//       error callbacks.
//
// See: https://www.khronos.org/opengl/wiki/OpenGL_Error
//
void Shader::Load(const char* vertPath, const char* fragPath)
{
    std::string vertSourceStr = ReadTextFile(vertPath);
    std::string fragSourceStr = ReadTextFile(fragPath);

    const char* vertSource = vertSourceStr.c_str();
    const char* fragSource = fragSourceStr.c_str();

    if (!vertSource)
    {
        Log::Warning(
            "ERROR: Vertex source %s failed to load!",
            vertPath
        );
    }
    if (!fragSource)
    {
        Log::Warning(
            "ERROR: Fragment source %s failed to load!",
            fragPath
        );
    }

    unsigned int vert, frag;

    // Vertex shader
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertSource, nullptr);
    glCompileShader(vert);
    CheckCompilerErrors(vert, ShaderType::Vertex);

    // Fragment shader
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSource, nullptr);
    glCompileShader(frag);
    CheckCompilerErrors(frag, ShaderType::Fragment);

    // Program
    ID = glCreateProgram();
    glAttachShader(ID, vert);
    glAttachShader(ID, frag);
    glLinkProgram(ID);
    CheckCompilerErrors(ID, ShaderType::Program);

    // Cleanup
    glDeleteShader(vert);
    glDeleteShader(frag);

    Log::Info("Shader %i compiled successfully!", ID);
}

void Shader::SetFloat(const char* name, float value)
{
    glUniform1f(
        glGetUniformLocation(ID, name),
        value
    );
}

void Shader::SetInt(const char* name, int value)
{
    glUniform1i(
        glGetUniformLocation(ID, name),
        value
    );
}

void Shader::SetVec2(const char* name, const glm::vec2& value)
{
    glUniform2f(
        glGetUniformLocation(ID, name),
        value.x,
        value.y
    );
}

void Shader::SetVec3(const char* name, const glm::vec3& value)
{
    glUniform3f(
        glGetUniformLocation(ID, name),
        value.x,
        value.y,
        value.z
    );
}

void Shader::SetVec4(const char* name, const glm::vec4& value)
{
    glUniform4f(
        glGetUniformLocation(ID, name),
        value.x,
        value.y,
        value.z,
        value.w
    );
}

void Shader::SetMat4(const char* name, const glm::mat4& value)
{
    glUniformMatrix4fv(
        glGetUniformLocation(ID, name),
        1,
        false,
        glm::value_ptr(value)
    );
}

std::string Shader::ReadTextFile(const char* path)
{
    std::string line, text;
    std::ifstream in(path);

    if (!in.is_open() || in.bad())
    {
        Log::Critical(
            "Failed to open file %s!",
            path
        );
    }

    while (std::getline(in, line))
    {
        text += line + "\n";
    }

    return text;
}

void Shader::CheckCompilerErrors(
    unsigned int object,
    Shader::ShaderType type)
{
    int success;
    char infoLog[1024];

    if (type != ShaderType::Program)
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            Log::Critical(
                "ERROR: Failed to compile %s shader. Error:\n%s \n",
                (type == ShaderType::Vertex) ? "Vertex" : "Fragment",
                infoLog
            );
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            Log::Critical(
                "ERROR: Failed to link shader. Error:\n%s \n",
                infoLog
            );
        }
    }
}
