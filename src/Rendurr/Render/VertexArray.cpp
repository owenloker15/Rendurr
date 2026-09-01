#include "VertexArray.h"

#include <glad/glad.h>

namespace rnd
{
    VertexArrayData vertex_array_create(std::vector<Vertex>&& vertices,
                                        std::vector<uint32_t>&& indices)
    {
        VertexArrayData data;

        data.vbData = vertex_buffer_create(std::move(vertices));
        data.ibData = index_buffer_create(std::move(indices));

        glCreateVertexArrays(1, &data.rendererId);

        // Vertex Buffer
        glVertexArrayVertexBuffer(data.rendererId, 0, data.vbData.rendererId, 0, sizeof(Vertex));

        glEnableVertexArrayAttrib(data.rendererId, 0); // Position
        glEnableVertexArrayAttrib(data.rendererId, 1); // Normals
        glEnableVertexArrayAttrib(data.rendererId, 2); // Texture Coordinates

        glVertexArrayAttribFormat(
            data.rendererId, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
        glVertexArrayAttribFormat(
            data.rendererId, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
        glVertexArrayAttribFormat(
            data.rendererId, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords));

        glVertexArrayAttribBinding(data.rendererId, 0, 0);
        glVertexArrayAttribBinding(data.rendererId, 1, 0);
        glVertexArrayAttribBinding(data.rendererId, 2, 0);

        // Index
        glVertexArrayElementBuffer(data.rendererId, data.ibData.rendererId);
        return data;
    }

    void vertex_array_destroy(uint32_t rendererId)
    {
        glDeleteVertexArrays(1, &rendererId);
    }

    void vertex_array_bind(uint32_t rendererId)
    {
        glBindVertexArray(rendererId);
    }

    void vertex_array_unbind()
    {
        glBindVertexArray(0);
    }

} // namespace rnd
