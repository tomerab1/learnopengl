#pragma once

struct GLFWwindow;

class Application
{
  public:
    explicit Application(GLFWwindow* window);

    void Run(bool* is_hot_reloaded);

  private:
    GLFWwindow* window;
};
