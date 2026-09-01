#pragma once

namespace rnd
{

#ifdef RND_ENABLE_ASSERTS

#if defined(_MSC_VER)
#define RND_DEBUGBREAK() __debugbreak()
#else
#include <signal.h>
#define RND_DEBUGBREAK() raise(SIGTRAP)
#endif

#define RND_ASSERT(condition, ...)                                                                 \
    do {                                                                                           \
        if (!(condition)) {                                                                        \
            RND_ERROR("Assertion Failed: {}", __VA_ARGS__);                                        \
            RND_DEBUGBREAK();                                                                      \
        }                                                                                          \
    } while (false)

#else
#define RND_ASSERT(condition, ...)
#endif

#define RND_TRACE(...)
#define RND_INFO(...)
#define RND_WARN(...)
#define RND_ERROR(...)
#define RND_CRITICAL(...)
} // namespace rnd
