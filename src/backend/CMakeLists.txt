set(BACKEND_LIB_NAME "GlfwBackendLib")
set(BACKEND_LIB_SRC "glfw_backend.cpp" "${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c")
set(BACKEND_LIB_HEADER "glfw_backend.h")

add_library(${BACKEND_LIB_NAME} STATIC ${BACKEND_LIB_SRC})
target_include_directories(${BACKEND_LIB_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/vendor/glad/include")
target_link_libraries(${BACKEND_LIB_NAME} PUBLIC glfw)