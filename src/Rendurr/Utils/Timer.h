#pragma once

#include <chrono>

namespace rnd
{
    inline std::chrono::time_point<std::chrono::high_resolution_clock> time_current()
    {
        return std::chrono::high_resolution_clock::now();
    }

    inline float time_delta(const std::chrono::time_point<std::chrono::high_resolution_clock> a,
                            const std::chrono::time_point<std::chrono::high_resolution_clock> b)
    {
        return (a - b).count();
    }
} // namespace rnd
