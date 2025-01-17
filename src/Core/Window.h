#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <memory>
#include <exception>
#include <chrono>

#include "Game/Game.h"

#define AA_SAMPLES      16

class Window
{
public:
    void InitWindow(int width, int height);
    void WindowLoop();

private:
    static void SizeChangeCallback(
        GLFWwindow* handle,
        int width,
        int height
    );

    static void KeyCallback(
        GLFWwindow* handle,
        int key,
        int scancode,
        int action,
        int mods
    );

public:
    static void GLAPIENTRY MessageCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam
    );

    GLFWwindow* GetWindowHandle();

protected:
    std::unique_ptr<Game> m_game;

private:
    void SetWindowIcon();

    GLFWwindow* m_handle;
    bool m_shouldClose = false;
};