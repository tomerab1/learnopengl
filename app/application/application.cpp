// clang-format off
#include <glad/glad.h>
// clang-format on
#include <GLFW/glfw3.h>

#include <iostream>
#include <spdlog/spdlog.h>

#include "config.h"
#include "shader_program.h"

// clang-format off
#include "application.h"
// clang-format on

struct RGBColor
{
    float r, g, b, alpha;

    static RGBColor GetRGB(int r, int g, int b, float alpha)
    {
        return RGBColor{r / 255.f, g / 255.f, b / 255.f, alpha};
    }
};

Application::Application(GLFWwindow* window)
  : window{window}
{
    assert(window != nullptr);
}

void Application::Run(bool* is_hot_reloaded)
{
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
        return;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    bool first = true;

    while (!glfwWindowShouldClose(window) && !*is_hot_reloaded)
    {
        const auto [r, g, b, a] = RGBColor::GetRGB(0x00, 0x0, 0xff, 1.f);

        if (first)
        {
            std::cout << r << " " << g << " " << b << "\n";
            first = false;
        }

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

extern "C" Application* CreateApplication(GLFWwindow* win)
{
    std::cout << "here create\n";
    return new Application(win);
}

extern "C" void DestroyApplication(Application* app)
{
    delete app;
}

extern "C" void RunApplication(Application* app, bool* is_hot_reloaded)
{
    assert(app != nullptr);

    app->Run(is_hot_reloaded);
}