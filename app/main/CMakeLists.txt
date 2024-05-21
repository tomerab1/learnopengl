set(EXE_SOURCE "main.cpp")
set(EXE_NAME "main")

add_executable(${EXE_NAME} ${EXE_SOURCE})
target_link_libraries(${EXE_NAME} PUBLIC GlfwBackendLib spdlog::spdlog nlohmann_json::nlohmann_json)
target_include_directories(${EXE_NAME} PUBLIC "${CMAKE_BINARY_DIR}/configured_files/include")