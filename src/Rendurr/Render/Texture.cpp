#include "Texture.h"

#include <Rendurr/Core/Log.h>
#include <cstdint>
#include <glad/glad.h>
#include <stb_image.h>

namespace rnd
{
    TextureData texture_create(const char* path, TextureType type)
    {
        TextureData data = {.name = path, .rendererId = 0, .type = type};

        stbi_set_flip_vertically_on_load(true);

        // Begin creating opengl texture
        glCreateTextures(GL_TEXTURE_2D, 1, &data.rendererId);

        glTextureParameteri(data.rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(data.rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(data.rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(data.rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Read texture data from file
        int width, height, numChannels;
        unsigned char* imgData = stbi_load(path, &width, &height, &numChannels, 0);

        if (imgData) {
            // glTexStorage2D(m_rendererId)

            // Check channels and determine format

            GLenum internalFormat = 0;
            GLenum dataFormat = 0;

            if (numChannels == 1) {
                internalFormat = GL_R8;
                dataFormat = GL_RED;
            }
            else if (numChannels == 3) {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            else if (numChannels == 4) {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else {
                RND_ERROR("Unsupported number of channels: {} for {}", numChannels, path);
            }

            glTextureStorage2D(data.rendererId, 1, internalFormat, width, height);
            glTextureSubImage2D(
                data.rendererId, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, imgData);

            // TODO error checking
        }
        else {
            const char* errorReason = stbi_failure_reason();
            RND_ERROR("Failed to load texture image data: {} - {}", path, errorReason);
        }

        stbi_image_free(imgData);
        return data;
    }

    TextureData texture_create_from_color(const glm::vec4& color)
    {
        TextureData texture{.name = "", .rendererId = 0, .type = TextureType::Unknown};

        glCreateTextures(GL_TEXTURE_2D, 1, &texture.rendererId);

        glBindTexture(GL_TEXTURE_2D, texture.rendererId);

        float pixelData[] = {color.r, color.g, color.b, color.a};

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1, 1, 0, GL_RGBA, GL_FLOAT, pixelData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);

        return texture;
    }

    void texture_destroy() {}

    void texture_bind(TextureData data, uint16_t textureSlot)
    {
        glBindTextureUnit(textureSlot, data.rendererId);
    }

    void texture_unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    TextureData unknown_texture()
    {
        return TextureData{.name = "Unknown", .rendererId = 0, .type = TextureType::Unknown};
    }

} // namespace rnd
