#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace rnd
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    struct VertexBufferData
    {
        uint32_t rendererId;
    };

    VertexBufferData vertex_buffer_create(std::vector<Vertex>&& vertices);
    void vertex_buffer_destroy(uint32_t rendererId);

} // namespace rnd
