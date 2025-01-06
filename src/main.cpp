#include <IO/Window.h>

int main()
{
    auto g_window = std::make_unique<Window>();
    g_window->CreateWindow(800, 600);
    g_window->WindowLoop();

    return 0;
}
