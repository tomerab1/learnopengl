#include "shader.h"

// clang-format off
#include <glad/glad.h>
// clang-format on
#include <cstring>
#include <sstream>

Shader::Shader(const path_t& shader_path, std::uint32_t shader_type)
  : path{shader_path}
  , shader_type{shader_type}
{
}

bool Shader::Compile()
{
    shader_program = glCreateShader(shader_type);
    std::ifstream input_stream;

    input_stream.exceptions(std::ios_base::badbit | std::ios_base::failbit);

    try
    {
        std::stringstream ss{};

        input_stream.open(path);
        ss << input_stream.rdbuf();

        auto shader_str = ss.str();
        char* shader_source = new char[shader_str.length()];
        std::memcpy(shader_source, shader_str.c_str(), shader_str.length());

        glShaderSource(shader_program, 1, &shader_source, nullptr);
        glCompileShader(shader_program);

        delete[] shader_source;

        int success;
        glGetShaderiv(shader_program, GL_COMPILE_STATUS, &success);

        return success;
    }
    catch (const std::exception& e)
    {
        std::memcpy((void*)error_buffer, e.what(), kErrorMaxSize);
        return false;
    }
}

std::uint32_t Shader::GetCompiledShader() const
{
    return shader_program;
}

const char* Shader::GetLastError()
{
    glGetShaderInfoLog(shader_program, kErrorMaxSize, nullptr, error_buffer);
    return error_buffer;
}

void Shader::Delete()
{
    glDeleteShader(shader_program);
}