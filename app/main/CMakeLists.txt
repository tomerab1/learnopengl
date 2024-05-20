set(EXE_SOURCE "main.cpp")
set(EXE_NAME "main")

add_executable(${EXE_NAME} ${EXE_SOURCE})
target_link_libraries(${EXE_NAME} PUBLIC "TempLib" "AppLib" GlfwBackendLib spdlog::spdlog)
target_include_directories(${EXE_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/app/application" ${CMAKE_SOURCE_DIR}/src/backend
 "${PROJECT_SOURCE_DIR}/vendor/glad/include" 
"${CMAKE_BINARY_DIR}/configured_files/include")