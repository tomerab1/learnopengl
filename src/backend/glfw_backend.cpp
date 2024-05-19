#include "glfw_backend.h"

// clang-format off
#include <glad/glad.h>
// clang-format on
#include <GLFW/glfw3.h>

#include <cassert>

GLFWBackend::GLFWBackend()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* GLFWBackend::CreateWindow(std::uint32_t width, std::uint32_t height, std::string_view title)
{
    pcurr_context = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    assert(pcurr_context);
    return pcurr_context;
}

void GLFWBackend::SetContextCurrent(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
}

bool GLFWBackend::InitGlad()
{
    bool ret = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    assert(ret != false);
    assert(pcurr_context != nullptr);

    std::int32_t width, height;
    glfwGetWindowSize(pcurr_context, &width, &height);

    glViewport(0, 0, width, height);
    return ret;
}

void GLFWBackend::setFrameBufferSizeCb(frame_buffer_size_cb cb)
{
    assert(pcurr_context != nullptr);
    glfwSetFramebufferSizeCallback(pcurr_context, cb);
}

void GLFWBackend::setKeyCb(key_press_cb cb)
{
    assert(pcurr_context != nullptr);
    glfwSetKeyCallback(pcurr_context, cb);
}

GLFWBackend::~GLFWBackend()
{
    glfwTerminate();
}