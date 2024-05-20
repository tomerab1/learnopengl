#include <cstdlib>
#include <dlfcn.h>
#include <iostream>
#include <thread>

// clang-format off
#include <glad/glad.h>
// clang-format on
#include "glfw_backend.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "config.h"

struct Application;

typedef Application* (*CreateApplicationFunc)(GLFWwindow*);
typedef void (*DestroyApplicationFunc)(Application**);
typedef void (*RunApplicationFunc)(Application*, bool*);

std::string libPath = "libAppLib.so";
std::string tmp_name = "TempLib";

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool should_hot_reload{false};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
    {
        should_hot_reload = true;
    }
}

bool loadLibrary(const char* libPath, void*& handle, CreateApplicationFunc* createApp, DestroyApplicationFunc* destroyApp, RunApplicationFunc* runApp)
{
    handle = dlopen(libPath, RTLD_NOW);
    if (!handle)
    {
        std::cerr << "Failed to load " << libPath << ": " << dlerror() << '\n';
        return false;
    }

    *createApp = (CreateApplicationFunc)dlsym(handle, "CreateApplication");
    *destroyApp = (DestroyApplicationFunc)dlsym(handle, "DestroyApplication");
    *runApp = (RunApplicationFunc)dlsym(handle, "RunApplication");

    if (!createApp || !destroyApp || !runApp)
    {
        std::cerr << "Failed to load symbols: " << dlerror() << '\n';
        dlclose(handle);
        return false;
    }
    return true;
}

void unloadLibrary(void*& handle)
{
    if (handle)
    {
        dlclose(handle);
        handle = nullptr;
    }
}

bool recompileLibrary()
{
    std::string command = "cd " + kProjectRootDirPath.string() +
                          " && cd build && cmake .." +
                          " && cmake --build . --target " + "AppLib";

    if (system(command.c_str()))
    {
        spdlog::error("Failed to recompile the library");
        return false;
    }

    return true;
}

int main()
{
    void* appLibHandle = nullptr;
    CreateApplicationFunc createApp = nullptr;
    DestroyApplicationFunc destroyApp = nullptr;
    RunApplicationFunc runApp = nullptr;

    if (!loadLibrary(libPath.c_str(), appLibHandle, &createApp, &destroyApp, &runApp))
    {
        return -1;
    }

    GLFWBackend glfw_backend;
    GLFWwindow* window = glfw_backend.CreateWindow(800, 600, "LearnOpenGL");

    if (!window)
    {
        spdlog::error("Failed to create GLFW window");
        unloadLibrary(appLibHandle);
        return -1;
    }

    glfw_backend.SetContextCurrent(window);

    if (!glfw_backend.InitGlad())
    {
        spdlog::error("Failed to initialize GLAD");
        unloadLibrary(appLibHandle);
        return -1;
    }

    glfw_backend.setFrameBufferSizeCb(framebufferSizeCallback);
    glfw_backend.setKeyCb(keyCallback);

    Application* app = createApp(window);

    while (!glfwWindowShouldClose(window))
    {
        if (should_hot_reload)
        {
            std::cout << "Hot reload triggered\n";

            destroyApp(&app);

            createApp = nullptr;
            destroyApp = nullptr;
            runApp = nullptr;
            app = nullptr;

            // Unload the current library
            unloadLibrary(appLibHandle);

            // Recompile the shared library
            if (!recompileLibrary())
            {
                spdlog::error("Failed to recompile library");
                break;
            }

            // Load the recompiled library
            if (!loadLibrary(libPath.c_str(), appLibHandle, &createApp, &destroyApp, &runApp))
            {
                spdlog::error("Failed to reload library");
                break;
            }

            // Recreate the Application instance
            app = createApp(window);
            should_hot_reload = false;
        }

        runApp(app, &should_hot_reload);
    }

    destroyApp(&app);

    unloadLibrary(appLibHandle);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
