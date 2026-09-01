#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace rnd
{
    enum class ColorAttachmentFormat
    {
        RGBA8
    };

    struct FramebufferAttachmentDesc
    {
        std::string key;
        ColorAttachmentFormat format;
    };

    struct FramebufferSpecification
    {
        uint32_t width = 0;
        uint32_t height = 0;

        std::vector<FramebufferAttachmentDesc> attachments;
    };

    struct Framebuffer
    {
        uint32_t rendererId = 0;

        uint32_t width = 0;
        uint32_t height = 0;

        uint32_t depthAttachment = 0;
        uint32_t colorAttachmentIndex = 0;

        std::unordered_map<std::string, uint32_t> colorAttachments;

        // REQUIRED for resize/rebuild correctness
        FramebufferSpecification spec;
    };

    Framebuffer framebuffer_create(const FramebufferSpecification& spec);
    void framebuffer_destroy(Framebuffer& fb);

    void framebuffer_bind(Framebuffer& fb);
    void framebuffer_unbind();

    void framebuffer_resize(Framebuffer& fb, uint32_t width, uint32_t height);

    uint32_t framebuffer_get_color_attachment(Framebuffer& fb, const std::string& key);
} // namespace rnd
