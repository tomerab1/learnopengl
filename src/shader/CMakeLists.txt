set(SHADER_LIB_NAME "ShaderLib")
set(SHADER_LIB_SRC "shader.cpp")
set(SHADER_LIB_HEADER "shader.h")

add_library(${SHADER_LIB_NAME} STATIC ${SHADER_LIB_SRC} ${SHADER_LIB_HEADER})
target_include_directories(${SHADER_LIB_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/vendor/glad/include")
target_link_libraries(${SHADER_LIB_NAME} PUBLIC spdlog::spdlog)