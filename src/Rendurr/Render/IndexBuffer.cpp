#include "IndexBuffer.h"

#include <glad/glad.h>

namespace rnd
{
    IndexBufferData index_buffer_create(std::vector<uint32_t>&& indices)
    {
        IndexBufferData data;

        data.indexCount = indices.size();
        glCreateBuffers(1, &data.rendererId);
        glNamedBufferData(data.rendererId,
                          sizeof(uint32_t) * indices.size(),
                          indices.data(),
                          GL_STATIC_DRAW);

        return data;
    }

    void index_buffer_destroy(uint32_t rendererId)
    {
        glDeleteBuffers(1, &rendererId);
    }
} // namespace rnd
