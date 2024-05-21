cmake_minimum_required(VERSION 3.22)
project(Learopengl VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(GLFW)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG  v1.14.1 
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    nlohmann_json 
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG  v3.11.3  
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_subdirectory(configured)
add_subdirectory(src)
add_subdirectory(app)