#pragma once

#include <glm/ext/vector_float4.hpp>

namespace rnd
{
    struct AssetManager;

    enum class TextureType : uint8_t
    {
        Unknown,
        Diffuse,
        Specular,
        Normal,
        Ambient,
        Roughness,
        Metallic,
        Emissive
    };

    struct TextureData
    {
        const char* name;
        uint32_t rendererId;
        TextureType type;
    };

    struct TextureUniformData
    {
        const char* uniformName;
        uint16_t textureSlot;
    };

    TextureData texture_create(const char* path, TextureType type);
    TextureData texture_create_from_color(const glm::vec4& color);
    void texture_destroy();
    void texture_bind(TextureData data, uint16_t textureSlot);
    void texture_unbind();

    TextureData unknown_texture();

} // namespace rnd
