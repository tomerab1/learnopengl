#include <cstdlib>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <thread>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "config.h"

typedef void (*RunApplicationFunc)();
typedef bool (*ShouldHotReloadFunc)();
typedef bool (*ShouldCloseAppFunc)();

RunApplicationFunc runAppFn = nullptr;
ShouldCloseAppFunc shouldCloseAppFn = nullptr;
ShouldHotReloadFunc shouldHotReloadFn = nullptr;

struct HotReloadFuncs
{
    void* fn;
    std::string sym;
};

HotReloadFuncs hotReloadFunc[] = {
    {.fn = nullptr, .sym = "RunApplication"},
    {.fn = nullptr, .sym = "ShouldCloseApp"},
    {.fn = nullptr, .sym = "ShouldHotReload"},
};

const std::string baseName = "AppLib";
std::string libName;
std::filesystem::path libPath;
nlohmann::json json;

bool loadLibrary(const char* libPath, void*& handle)
{
    handle = dlopen(libPath, RTLD_NOW | RTLD_LOCAL);
    if (!handle)
    {
        spdlog::error("Failed to load {} : {}", libPath, dlerror());
        return false;
    }

    for (auto& entry : hotReloadFunc)
    {
        entry.fn = dlsym(handle, entry.sym.c_str());

        if (!entry.fn)
        {
            spdlog::error("Failed to load symbol : {}", dlerror());
            dlclose(handle);
            return false;
        }
    }

    return true;
}

void unloadLibrary(void*& handle)
{
    if (handle)
    {
        for (auto& entry : hotReloadFunc)
        {
            entry.fn = nullptr;
        }

        dlclose(handle);
        handle = nullptr;
    }
}

bool recompileLibrary()
{
    try
    {
        std::filesystem::remove(libPath);
    }
    catch (const std::exception& e)
    {
        spdlog::error("{}", e.what());
        return false;
    }

    libName = baseName + std::to_string(std::chrono::seconds(std::time(NULL)).count());
    libPath = libPath.parent_path() / ("lib" + libName + ".so");

    json["libName"] = libName;
    json["libPath"] = libPath;

    std::ofstream outStream{kProjectRootDirPath / "hot-reload.json", std::fstream::binary};
    outStream << nlohmann::to_string(json);

    std::string command = "cd " + kProjectRootDirPath.string() +
                          " && cd build && cmake -DNEW_LIB_NAME=" + libName + " .. " +
                          " && cmake --build . --target " + libName;

    if (system(command.c_str()))
    {
        spdlog::error("Failed to recompile the library");
        return false;
    }

    return true;
}

int main()
{
    if (!std::filesystem::exists(kProjectRootDirPath / "hot-reload.json"))
    {
        std::cout << "here\n";
        std::ofstream outStream{kProjectRootDirPath / "hot-reload.json"};
        const std::string firstLoadJson = R"({
            "libPath": "/home/tomerab/VSCProjects/learnopengl/build/app/application/libAppLib.so",
            "libName": "AppLib"
        })";

        outStream << firstLoadJson;
    }

    std::ifstream inStream{kProjectRootDirPath / "hot-reload.json"};
    std::stringstream ss{};

    ss << inStream.rdbuf();

    json = nlohmann::json::parse(ss.str());

    libPath = std::move(std::filesystem::path(json["libPath"]));
    libName = std::move(json["libName"]);

    void* appLibHandle = nullptr;

    if (!loadLibrary(libPath.c_str(), appLibHandle))
    {
        return -1;
    }

    while (true)
    {
        if (((ShouldHotReloadFunc)hotReloadFunc[2].fn)())
        {
            unloadLibrary(appLibHandle);
            recompileLibrary();
            loadLibrary(libPath.c_str(), appLibHandle);
        }
        if (((ShouldCloseAppFunc)hotReloadFunc[1].fn)())
        {
            break;
        }

        ((RunApplicationFunc)hotReloadFunc[0].fn)();
    }

    unloadLibrary(appLibHandle);

    return 0;
}
