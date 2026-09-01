#pragma once

#include <glm/vec4.hpp>

namespace rnd
{
    void clear();
    void setClearColor(const glm::vec4& rgba);
    void setViewport(float width, float height);
    void enableDepthTesting();
    void enableDebug();

    void drawIndexed(uint32_t vertexArrayRendererId, uint32_t indexCount);
} // namespace rnd
