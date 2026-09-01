#pragma once

#include <GLFW/glfw3.h>

#include "Input.h"

namespace rnd
{
    struct WindowSpec
    {
        const char* title;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    struct Window
    {
        WindowSpec specification;
        GLFWwindow* glfwWindow;
    };

    Window window_create(WindowSpec specification);
    void window_destroy(const Window& window);
    void window_swap_buffers(const Window& window);
    void window_retrieve_events(const Window& window, Input& input);
    float get_window_width(const Window& window);
    float get_window_height(const Window& window);

    void poll_events();

} // namespace rnd
