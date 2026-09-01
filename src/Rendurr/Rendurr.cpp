#include "Rendurr.h"

#include "Core/Log.h"
#include "GLFW/glfw3.h"

namespace rnd
{
    void init()
    {
        if (!glfwInit()) {
            RND_ERROR("Failed to initialize GLFW!");
            glfwTerminate();
        }
    }
} // namespace rnd
