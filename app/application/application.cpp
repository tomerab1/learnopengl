// clang-format off
#include "application.h"
// clang-format on

#include "config.h"
#include "glfw_backend.h"
#include "shader_program.h"

// clang-format off
#include <glad/glad.h>
// clang-format on
#include <GLFW/glfw3.h>

#include <iostream>
#include <spdlog/spdlog.h>
#include <stdio.h>

struct RGBColor
{
    float r, g, b, alpha;

    static RGBColor GetRGB(int r, int g, int b, float alpha)
    {
        return RGBColor{r / 255.f, g / 255.f, b / 255.f, alpha};
    }
};

static bool shouldHotReload{false};
static bool shouldCloseApp{false};

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        shouldCloseApp = true;
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
    {
        shouldHotReload = true;
    }
}

extern "C" bool ShouldHotReload()
{
    return shouldHotReload;
}

extern "C" bool ShouldCloseApp()
{
    return shouldCloseApp;
}

extern "C" void RunApplication()
{
    shouldHotReload = false;
    GLFWBackend glfwBackend;

    GLFWwindow* window = glfwBackend.CreateWindow(800, 600, "learnopengl");

    glfwBackend.SetContextCurrent(window);
    if (!glfwBackend.InitGlad())
    {
        spdlog::error("Failed to init GLAD");
        return;
    }

    glfwBackend.setFrameBufferSizeCb(framebufferSizeCallback);
    glfwBackend.setKeyCb(keyCallback);

    // clang-format off
    float vertices[] = {
                        0.5f, 0.5f, 0.f,
                        0.5f, -0.5f, 0.f,
                        -0.5f, -0.5f, 0.f,
                        -0.5f, 0.5f, 0.f
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
        return;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window) && !shouldHotReload)
    {
        const auto [r, g, b, a] = RGBColor::GetRGB(0x1e, 0xf3, 0x73, 1);

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
    glDeleteBuffers(1, &ebo);
    shader_program.Delete();
}