#pragma once

#include <filesystem>
#include <fstream>

class Shader
{
  public:
    using path_t = std::filesystem::path;

    explicit Shader(const path_t& shader_path, std::uint32_t shader_type);

    bool Compile();
    std::uint32_t GetCompiledShader() const;
    const char* GetLastError();
    void Delete();

    ~Shader();

  private:
    static constexpr std::size_t kErrorMaxSize{512};
    char error_buffer[kErrorMaxSize]{};

    path_t path;
    std::uint32_t shader_type;
    std::uint32_t shader_program{};
};