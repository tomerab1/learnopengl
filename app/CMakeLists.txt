
set(EXE_SOURCES "main.cpp" "${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c")
set(EXE_INCLUDES "${PROJECT_SOURCE_DIR}/vendor/glad/include" "${CMAKE_BINARY_DIR}/configured_files/include")
set(EXE_NAME "main")

add_executable(${EXE_NAME} ${EXE_SOURCES})
target_link_libraries(${EXE_NAME} PUBLIC glfw spdlog::spdlog ShaderLib)
target_include_directories(${EXE_NAME} PUBLIC ${EXE_INCLUDES} ${CMAKE_SOURCE_DIR}/src/shader)