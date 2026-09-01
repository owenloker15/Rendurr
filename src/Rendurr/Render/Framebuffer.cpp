#include "Framebuffer.h"

#include <Rendurr/Core/Log.h>
#include <glad/glad.h>

namespace
{
    void gl_format(rnd::ColorAttachmentFormat fmt, GLint* internal, GLenum* format)
    {
        switch (fmt) {
            case rnd::ColorAttachmentFormat::RGBA8:
                *internal = GL_RGBA8;
                *format = GL_RGBA;
                break;

            default:
                RND_ERROR("Invalid ColorAttachmentFormat");
                *internal = GL_RGBA8;
                *format = GL_RGBA;
                break;
        }
    }

    void framebuffer_destroy_internal(rnd::Framebuffer& fb)
    {
        if (fb.rendererId)
            glDeleteFramebuffers(1, &fb.rendererId);

        for (auto& it : fb.colorAttachments)
            glDeleteTextures(1, &it.second);

        if (fb.depthAttachment)
            glDeleteRenderbuffers(1, &fb.depthAttachment);
    }

    void framebuffer_build(rnd::Framebuffer& fb)
    {
        fb.colorAttachmentIndex = 0;

        glBindFramebuffer(GL_FRAMEBUFFER, fb.rendererId);

        std::vector<GLenum> drawBuffers;
        drawBuffers.reserve(fb.spec.attachments.size());

        for (size_t i = 0; i < fb.spec.attachments.size(); i++) {
            const auto& a = fb.spec.attachments[i];

            GLint internal;
            GLenum format;
            gl_format(a.format, &internal, &format);

            uint32_t tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);

            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         internal,
                         fb.width,
                         fb.height,
                         0,
                         format,
                         GL_UNSIGNED_BYTE,
                         nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i, GL_TEXTURE_2D, tex, 0);

            fb.colorAttachments[a.key] = tex;
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + (GLenum)i);

            fb.colorAttachmentIndex++;
        }

        glDrawBuffers((GLsizei)drawBuffers.size(), drawBuffers.data());

        // depth buffer
        glGenRenderbuffers(1, &fb.depthAttachment);
        glBindRenderbuffer(GL_RENDERBUFFER, fb.depthAttachment);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fb.width, fb.height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER,
                                  fb.depthAttachment);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            RND_ERROR("Framebuffer incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
} // namespace

namespace rnd
{
    Framebuffer framebuffer_create(const FramebufferSpecification& spec)
    {
        Framebuffer fb{};

        fb.spec = spec;
        fb.width = spec.width;
        fb.height = spec.height;

        glGenFramebuffers(1, &fb.rendererId);

        framebuffer_build(fb);

        return fb;
    }

    void framebuffer_destroy(Framebuffer& fb)
    {
        framebuffer_destroy_internal(fb);

        fb.colorAttachments.clear();
        fb.rendererId = 0;
        fb.depthAttachment = 0;
        fb.width = 0;
        fb.height = 0;
    }

    void framebuffer_bind(Framebuffer& fb)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fb.rendererId);
        glViewport(0, 0, fb.width, fb.height);
    }

    void framebuffer_unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void framebuffer_resize(Framebuffer& fb, uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0)
            return;

        fb.width = width;
        fb.height = height;

        // delete old GPU resources
        for (auto& it : fb.colorAttachments)
            glDeleteTextures(1, &it.second);

        fb.colorAttachments.clear();

        if (fb.depthAttachment)
            glDeleteRenderbuffers(1, &fb.depthAttachment);

        fb.depthAttachment = 0;

        framebuffer_build(fb);
    }

    uint32_t framebuffer_get_color_attachment(Framebuffer& fb, const char* key)
    {
        auto it = fb.colorAttachments.find(key);
        if (it == fb.colorAttachments.end())
            return 0;

        return it->second;
    }
} // namespace rnd
