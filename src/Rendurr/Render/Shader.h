#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace rnd
{
    struct Shader
    {
        const char* vertexFilePath;
        const char* fragmentFilePath;
        uint32_t rendererId;
    };

    Shader shader_program_create(const char* vertexFilePath, const char* fragmentFilePath);
    void shader_program_destroy(const Shader& shader);
    void shader_program_use(const Shader& shader);

    // Uniform uploads
    void shader_uniform_upload_bool(const Shader& shader, const char* uniformName, bool value);
    void shader_uniform_upload_int(const Shader& shader, const char* uniformName, int value);
    void shader_uniform_upload_float(const Shader& shader, const char* uniformName, float value);
    void shader_uniform_upload_vec2(const Shader& shader,
                                    const char* uniformName,
                                    const glm::vec2& value);
    void shader_uniform_upload_vec3(const Shader& shader,
                                    const char* uniformName,
                                    const glm::vec3& value);
    void shader_uniform_upload_vec4(const Shader& shader,
                                    const char* uniformName,
                                    const glm::vec4& value);
    void shader_uniform_upload_mat3(const Shader& shader,
                                    const char* uniformName,
                                    const glm::mat3& value);
    void shader_uniform_upload_mat4(const Shader& shader,
                                    const char* uniformName,
                                    const glm::mat4& value);

} // namespace rnd
