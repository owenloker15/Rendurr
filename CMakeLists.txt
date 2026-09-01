cmake_minimum_required(VERSION 4.1)

# Create project
project(Rendurr)

# Use C++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ThirdParty/glad)
add_subdirectory(ThirdParty/glfw)
add_subdirectory(ThirdParty/glm)
add_subdirectory(ThirdParty/stb_image)

# Set sources
set(RENDURR_SOURCES
    src/Rendurr/Rendurr.h
    src/Rendurr/Rendurr.cpp

    src/Rendurr/Core/Input.h
    src/Rendurr/Core/Log.h
    src/Rendurr/Core/Window.h
    src/Rendurr/Core/Window.cpp

    src/Rendurr/Render/Framebuffer.h
    src/Rendurr/Render/Framebuffer.cpp
    src/Rendurr/Render/IndexBuffer.h
    src/Rendurr/Render/IndexBuffer.cpp
    src/Rendurr/Render/Renderer.h
    src/Rendurr/Render/Renderer.cpp
    src/Rendurr/Render/Shader.h
    src/Rendurr/Render/Shader.cpp
    src/Rendurr/Render/Texture.h
    src/Rendurr/Render/Texture.cpp
    src/Rendurr/Render/VertexArray.h
    src/Rendurr/Render/VertexArray.cpp
    src/Rendurr/Render/VertexBuffer.h
    src/Rendurr/Render/VertexBuffer.cpp

    src/Rendurr/Scene/Camera.h

    src/Rendurr/Utils/FileUtils.h
    src/Rendurr/Utils/Timer.h
)

# Create static library
add_library(Rendurr STATIC ${RENDURR_SOURCES})

# Set ThirdParty Dir
set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty)

# Set include directory as src
target_include_directories(Rendurr PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Rendurr PRIVATE
    glad
    glfw
    glm::glm
    stb_image
)

# Compile Defs

# Prevents glfw from including any opengl headers
target_compile_definitions(Rendurr PUBLIC GLFW_INCLUDE_NONE)
target_compile_definitions(Rendurr
    PRIVATE
        $<$<CONFIG:Debug>:RND_ENABLE_ASSERTS>
)
# End compile defs
