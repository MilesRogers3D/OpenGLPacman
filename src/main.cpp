#include <IO/Window.h>

int main()
{
    Window g_window;

    g_window.InitWindow(896, 1152);
    g_window.WindowLoop();

    return 0;
}
