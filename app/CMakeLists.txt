
set(EXE_SOURCES "main.cpp" "${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c")
set(EXE_NAME "main")

add_executable(${EXE_NAME} ${EXE_SOURCES})
target_link_libraries(${EXE_NAME} PUBLIC glfw)
target_include_directories(${EXE_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/vendor/glad/include")