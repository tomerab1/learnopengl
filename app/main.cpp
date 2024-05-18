// clang-format off
#include <glad/glad.h>
// clang-format on
#include <GLFW/glfw3.h>

#include <iostream>
#include <spdlog/spdlog.h>

#include "config.h"
#include "shader.h"

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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Learnopengl", nullptr, nullptr);
    if (!window)
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

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

    Shader vertex_shader{kProjectRootDirPath / "shaders" / "vertex_shader.glsl", GL_VERTEX_SHADER};

    if (!vertex_shader.Compile())
    {
        spdlog::error("Failed to compile vertex shader: {}", vertex_shader.GetLastError());
        return -1;
    }

    Shader fragment_shader{kProjectRootDirPath / "shaders" / "fragment_shader.glsl", GL_FRAGMENT_SHADER};

    if (!fragment_shader.Compile())
    {
        spdlog::error("Failed to compile fragment shader: {}", fragment_shader.GetLastError());
        return -1;
    }

    std::uint32_t shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader.GetCompiledShader());
    glAttachShader(shaderProgram, fragment_shader.GetCompiledShader());
    glLinkProgram(shaderProgram);

    vertex_shader.Delete();
    fragment_shader.Delete();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        const auto [r, g, b, a] = RGBColor::GetRGB(0x28, 0x2b, 0x2e, 1.f);

        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}