// clang-format off
#include <glad/glad.h>
// clang-format on
#include <GLFW/glfw3.h>

#include <iostream>
#include <spdlog/spdlog.h>

#include "config.h"
#include "glfw_backend.h"
#include "shader_program.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

struct RGBColor
{
    float r, g, b, alpha;

    static RGBColor GetRGB(int r, int g, int b, float alpha)
    {
        return RGBColor{r / 255.f, g / 255.f, b / 255.f, alpha};
    }
};

int main()
{
    GLFWBackend glfw_backend;

    GLFWwindow* window = glfw_backend.CreateWindow(800, 600, "Learnopengl");

    if (!window)
    {
        spdlog::error("Failed to create GLFW window");
        return -1;
    }

    glfw_backend.SetContextCurrent(window);

    if (!glfw_backend.InitGlad())
    {
        spdlog::error("Failed to initialize GLAD");
        return -1;
    }

    glfw_backend.setFrameBufferSizeCb(framebufferSizeCallback);
    glfw_backend.setKeyCb(keyCallback);

    // clang-format off
    float vertices[] = {
                        0.25f, 0.25f, 0.f,
                        0.25f, -0.25f, 0.f,
                        -0.25f, -0.25f, 0.f,
                        -0.25f, 0.25f, 0.f
                     };
    
    std::uint32_t indices[] = {
        0, 3, 1,
        1, 2, 3
    };
    // clang-format on

    std::uint32_t vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ShaderProgram shader_program{kProjectRootDirPath / "shaders" / "vertex_shader.glsl",
                                 kProjectRootDirPath / "shaders" / "fragment_shader.glsl"};

    if (!shader_program.Link())
    {
        spdlog::error("Failed to create a shader program: {}", shader_program.GetLastError());
        return -1;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        const auto [r, g, b, a] = RGBColor::GetRGB(0x0d, 0x11, 0x17, 1.f);

        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.UseProgram();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    shader_program.Delete();

    return 0;
}