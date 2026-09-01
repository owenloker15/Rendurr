#include "Shader.h"

#include <Rendurr/Utils/FileUtils.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace
{
    uint32_t compileShader(GLenum shaderType, const char* shaderSource)
    {
        // Create shader object
        uint32_t shader = glCreateShader(shaderType);

        // Attach the shader source to the shader object and compile the shader
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);

        GLint success = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (success != GL_TRUE) {
            char infoLog[4096];
            GLsizei length = 0;

            glGetShaderInfoLog(shader, sizeof(infoLog), &length, infoLog);

            RND_ERROR("Shader compilation failed:\n{}", infoLog);

            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    GLint getUniformLocation(uint32_t shaderId, const char* uniformName)
    {
        auto uniformLocation = glGetUniformLocation(shaderId, uniformName);
        if (uniformLocation == -1) {
            RND_ERROR("Failed to find uniform with name {} in shader with id {}!",
                      uniformName,
                      shaderId);
        }
        return uniformLocation;
    }
} // namespace

namespace rnd
{
    Shader shader_program_create(const char* vertexFilePath, const char* fragmentFilePath)
    {
        const char* vertexShaderContent = Utils::read_file(vertexFilePath);
        const char* fragmentShaderContent = Utils::read_file(fragmentFilePath);

        // Compile vertex and fragment shaders
        uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderContent);
        uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderContent);

        // Link the compiled shaders to the shader program
        uint32_t rendererId = glCreateProgram();
        glAttachShader(rendererId, vertexShader);
        glAttachShader(rendererId, fragmentShader);
        glLinkProgram(rendererId);

        // Check for errors
        int success;
        char infoLog[512];
        glGetProgramiv(rendererId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(rendererId, 512, NULL, infoLog);
            RND_ERROR("Failed to link shader program!", infoLog);
        }

        // Cleanup shaders since they are already linked to the shader program
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        Shader shader;
        shader.vertexFilePath = vertexFilePath;
        shader.fragmentFilePath = fragmentFilePath;
        shader.rendererId = rendererId;

        return shader;
    }

    void shader_program_destroy(const Shader& shader)
    {
        glDeleteProgram(shader.rendererId);
    }

    void shader_program_use(const Shader& shader)
    {
        glUseProgram(shader.rendererId);
    }

    void shader_uniform_upload_bool(const Shader& shader, const char* uniformName, bool value)
    {
        shader_program_use(shader);
        GLint location = getUniformLocation(shader.rendererId, uniformName);
        glUniform1i(location, value ? 1 : 0);
    }

    void shader_uniform_upload_int(const Shader& shader, const char* uniformName, int value)
    {
        shader_program_use(shader);
        GLint location = getUniformLocation(shader.rendererId, uniformName);
        glUniform1i(location, value);
    }

    void shader_uniform_upload_float(const Shader& shader, const char* uniformName, float value)
    {
        shader_program_use(shader);
        GLint location = getUniformLocation(shader.rendererId, uniformName);
        glUniform1f(location, value);
    }

    void shader_uniform_upload_vec2(const Shader& shader,
                                    const char* uniformName,
                                    const glm::vec2& value)
    {
        shader_program_use(shader);
        GLint location = getUniformLocation(shader.rendererId, uniformName);
        glUniform2fv(location, 1, glm::value_ptr(value));
    }

    void shader_uniform_upload_vec3(const Shader& shader,
                                    const char* uniformName,
                                    const glm::vec3& value)
    {
        shader_program_use(shader);
        GLint location = getUniformLocation(shader.rendererId, uniformName);
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    void shader_uniform_upload_vec4(const Shader& shader,
                                    const char* uniformName,
                                    const glm::vec4& value)
    {
        shader_program_use(shader);
        GLint location = getUniformLocation(shader.rendererId, uniformName);
        glUniform4fv(location, 1, glm::value_ptr(value));
    }

    void shader_uniform_upload_mat3(const Shader& shader,
                                    const char* uniformName,
                                    const glm::mat3& value)
    {
        shader_program_use(shader);
        GLint location = getUniformLocation(shader.rendererId, uniformName);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void shader_uniform_upload_mat4(const Shader& shader,
                                    const char* uniformName,
                                    const glm::mat4& value)
    {
        shader_program_use(shader);
        GLint location = getUniformLocation(shader.rendererId, uniformName);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
} // namespace rnd
