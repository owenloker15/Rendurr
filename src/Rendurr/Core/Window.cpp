#include "Window.h"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstring>
#include <glad/glad.h>

#include "Log.h"

namespace rnd
{
    struct ScrollAccumulator
    {
        GLFWwindow* window;
        float deltaX, deltaY;
    };

    static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        ScrollAccumulator* accum =
            static_cast<ScrollAccumulator*>(glfwGetWindowUserPointer(window));

        accum->deltaX += static_cast<float>(xoffset);
        accum->deltaY += static_cast<float>(yoffset);
    }

    Window window_create(WindowSpec specification)
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        const auto glfwWindow = glfwCreateWindow(
            specification.width, specification.height, specification.title, nullptr, nullptr);

        if (!glfwWindow) {
            RND_ERROR("Failed to create window!");
            glfwTerminate();
        }

        glfwMakeContextCurrent(glfwWindow);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            RND_ERROR("Failed to initialize glad!");
        }

        // Pass a pointer to glfw to hold onto. Gets freed on window_destroy
        ScrollAccumulator* accum = new ScrollAccumulator();
        glfwSetWindowUserPointer(glfwWindow, accum);
        glfwSetScrollCallback(glfwWindow, glfw_scroll_callback);

        Window window = {.specification = specification, .glfwWindow = glfwWindow};
        return window;
    }

    void window_destroy(const Window& window)
    {
        auto userPointer = glfwGetWindowUserPointer(window.glfwWindow);
        free(userPointer);
        glfwDestroyWindow(window.glfwWindow);
    }

    void window_swap_buffers(const Window& window)
    {
        glfwSwapBuffers(window.glfwWindow);
    }

    void poll_events()
    {
        glfwPollEvents();
    }

    void window_retrieve_events(const Window& window, Input& input)
    {
        input.windowCloseRequested = glfwWindowShouldClose(window.glfwWindow);

        // Clear transient states
        memset(input.keysPressed, 0, sizeof(input.keysPressed));
        memset(input.keysReleased, 0, sizeof(input.keysReleased));

        input.mouseButtonsPressed = 0;
        input.mouseButtonsReleased = 0;

        ScrollAccumulator* accum =
            static_cast<ScrollAccumulator*>(glfwGetWindowUserPointer(window.glfwWindow));
        input.scrollDeltaX = accum->deltaX;
        input.scrollDeltaY = accum->deltaY;

        accum->deltaX = 0.0f;
        accum->deltaY = 0.0f;

        // Keyboard input
        for (uint16_t key = 0; key <= rnd::MaxKeyCode; ++key) {
            bool wasDown = get_bit(input.keysDown, key);
            bool isDown = glfwGetKey(window.glfwWindow, key) == GLFW_PRESS;

            if (isDown && !wasDown)
                set_bit(input.keysPressed, key, true);

            if (!isDown && wasDown)
                set_bit(input.keysReleased, key, true);

            set_bit(input.keysDown, key, isDown);
        }

        // Mouse buttons
        for (uint16_t button = 0; button <= GLFW_MOUSE_BUTTON_LAST; ++button) {
            bool wasDown = (input.mouseButtonsDown & (1u << button)) != 0;

            bool isDown = glfwGetMouseButton(window.glfwWindow, button) == GLFW_PRESS;

            if (isDown && !wasDown)
                input.mouseButtonsPressed |= (1u << button);

            if (!isDown && wasDown)
                input.mouseButtonsReleased |= (1u << button);

            if (isDown)
                input.mouseButtonsDown |= (1u << button);
            else
                input.mouseButtonsDown &= ~(1u << button);
        }

        // Mouse position
        input.prevMouseXPos = input.mouseXPos;
        input.prevMouseYPos = input.mouseYPos;

        double x, y;
        glfwGetCursorPos(window.glfwWindow, &x, &y);

        input.mouseXPos = static_cast<float>(x);
        input.mouseYPos = static_cast<float>(y);

        input.deltaX = input.mouseXPos - input.prevMouseXPos;
        input.deltaY = input.mouseYPos - input.prevMouseYPos;
    }

    float get_window_width(const Window& window)
    {
        int width, height;
        glfwGetWindowSize(window.glfwWindow, &width, &height);
        return width;
    }

    float get_window_height(const Window& window)
    {
        int width, height;
        glfwGetWindowSize(window.glfwWindow, &width, &height);
        return height;
    }

} // namespace rnd
