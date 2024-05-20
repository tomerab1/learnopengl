#pragma once

struct GLFWwindow;
struct RGBColor;

class Application
{
  public:
    explicit Application(GLFWwindow* window);
    GLFWwindow* GetWindow() const;

  private:
    GLFWwindow* window;
};
