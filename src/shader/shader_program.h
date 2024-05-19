#pragma once

#include <filesystem>
#include <memory>

class Shader;

class ShaderProgram
{
  public:
    using path_t = std::filesystem::path;

    explicit ShaderProgram(const path_t& vertex_path, const path_t& fragment_path);

    bool Link() const;
    void UseProgram();
    void Delete();
    const char* GetLastError();

    ~ShaderProgram();

  private:
    std::unique_ptr<Shader> vertex_shader;
    std::unique_ptr<Shader> fragment_shader;
    mutable std::uint32_t shader_program{};

    static constexpr std::size_t kErrorMaxSize{512};
    char error_buffer[kErrorMaxSize]{};
};