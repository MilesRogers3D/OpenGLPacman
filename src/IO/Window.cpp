#include "Window.h"

#include "IO/Log.h"

void Window::SizeChangeCallback(
    GLFWwindow* handle,
    int width,
    int height)
{
    glViewport(0, 0, width, height);

    auto windowPtr = static_cast<Window*>(
        glfwGetWindowUserPointer(handle)
    );

    windowPtr->m_game->OnWindowResize(width, height);
}

void Window::KeyCallback(
    GLFWwindow* handle,
    int key,
    int scancode,
    int action,
    int mods)
{
    // Invalid key input
    if (key < 0)
    {
        return;
    }

    auto windowPtr = static_cast<Window*>(
        glfwGetWindowUserPointer(handle)
    );

    switch(action)
    {
        case GLFW_PRESS:
            windowPtr->m_game->OnKeyPressed(key);
        case GLFW_RELEASE:
            windowPtr->m_game->OnKeyReleased(key);
        default:
            break;
    }
}

void Window::MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    if (type != GL_DEBUG_TYPE_ERROR)
    {
        return;
    }

    if (severity == GL_DEBUG_SEVERITY_LOW)
    {
        return;
    }

    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (
            type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

void Window::InitWindow(int width, int height)
{
    if (!glfwInit())
    {
        throw std::exception(
        "Unable to initialize GLFW!"
        );
        exit(1);
    }

    // Using 4.3 for debugger callbacks
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    // Enable MSAA (GLFW)
    glfwWindowHint(GLFW_SAMPLES, AA_SAMPLES);

    m_handle = glfwCreateWindow(
        width,
        height,
        "OpenGL Pacman",
        nullptr,
        nullptr
    );

    if (m_handle == nullptr)
    {
        throw std::exception(
            "Unable to create GLFW window!"
        );
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(m_handle);
    glfwSetWindowUserPointer(m_handle, this);

    glfwSetFramebufferSizeCallback(
        m_handle,
        SizeChangeCallback
    );
    glfwSetKeyCallback(
        m_handle,
        KeyCallback
    );

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::exception(
        "Unable to initialize GLAD!"
        );
        exit(1);
    }

    // Enable debug callbacks
#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
#endif

    glfwSwapInterval(1);

    // Enable MSAA (glad)
    glEnable(GL_MULTISAMPLE);
}

void Window::WindowLoop()
{
    m_game = std::make_unique<Game>();
    m_game->Init();
    auto prevTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(m_handle) &&
           !m_shouldClose)
    {
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        // Call engine loops
        m_game->Update(deltaTime.count());
        m_game->Render();

        // Refresh window
        glfwSwapBuffers(m_handle);
        glfwPollEvents();
    }

    m_game->Destroy();
}


