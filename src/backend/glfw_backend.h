#pragma once

#include <string_view>

struct GLFWwindow;

typedef void (*frame_buffer_size_cb)(GLFWwindow* window, int width, int height);
typedef void (*key_press_cb)(GLFWwindow* window, int key, int scancode, int action, int mods);

class GLFWBackend
{
  public:
    GLFWBackend();

    GLFWwindow* CreateWindow(std::uint32_t width, std::uint32_t height, std::string_view title);
    void SetContextCurrent(GLFWwindow* window);
    bool InitGlad();
    void setFrameBufferSizeCb(frame_buffer_size_cb cb);
    void setKeyCb(key_press_cb cb);

    ~GLFWBackend();

  private:
    GLFWwindow* pcurr_context{nullptr};
};