#pragma once

#include <stdint.h>
#include <vector>

namespace rnd
{
    struct IndexBufferData
    {
        uint32_t rendererId = 0;
        uint32_t indexCount = 0;
    };

    IndexBufferData index_buffer_create(std::vector<uint32_t>&& indices);
    void index_buffer_destroy(uint32_t rendererId);

} // namespace rnd
