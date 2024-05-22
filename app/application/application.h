#pragma once

#include <memory>

struct GLFWwindow;
struct GLFWBackend;

class Application
{
  public:
    Application(std::uint32_t width, std::uint32_t height, std::string_view title);

    void Run();

    ~Application();

  private:
    std::unique_ptr<GLFWBackend> glfwBackend;
    GLFWwindow* window;
};