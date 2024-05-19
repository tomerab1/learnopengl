#include "shader_program.h"

#include "shader.h"
#include <cstring>
#include <glad/glad.h>

ShaderProgram::ShaderProgram(const path_t& vertex_path, const path_t& fragment_path)
  : vertex_shader{std::make_unique<Shader>(vertex_path, GL_VERTEX_SHADER)}
  , fragment_shader{std::make_unique<Shader>(fragment_path, GL_FRAGMENT_SHADER)}
{
}

bool ShaderProgram::Link() const
{
    if (!vertex_shader->Compile())
    {
        std::memcpy((void*)error_buffer, vertex_shader->GetLastError(), kErrorMaxSize);
        return false;
    }

    if (!fragment_shader->Compile())
    {
        std::memcpy((void*)error_buffer, fragment_shader->GetLastError(), kErrorMaxSize);
        return false;
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader->GetCompiledShader());
    glAttachShader(shader_program, fragment_shader->GetCompiledShader());
    glLinkProgram(shader_program);

    vertex_shader->Delete();
    fragment_shader->Delete();

    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    return success;
}

const char* ShaderProgram::GetLastError()
{
    glad_glGetProgramInfoLog(shader_program, kErrorMaxSize, nullptr, error_buffer);
    return error_buffer;
}

void ShaderProgram::UseProgram()
{
    glUseProgram(shader_program);
}

void ShaderProgram::Delete()
{
    glDeleteProgram(shader_program);
}

ShaderProgram::~ShaderProgram() = default;