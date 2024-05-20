set(APP_LIB1_NAME "AppLib")
set(APP_LIB2_NAME "TempLib")

set(APP_LIB_SRC "application.cpp" "${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c" )
set(APP_LIB_HEADER "application.h" 
"${PROJECT_SOURCE_DIR}/vendor/glad/include" 
"${CMAKE_BINARY_DIR}/configured_files/include")

add_library(${APP_LIB1_NAME} SHARED ${APP_LIB_SRC})
target_link_libraries(${APP_LIB1_NAME} PUBLIC glfw spdlog::spdlog ShaderLib)
target_include_directories(${APP_LIB1_NAME} PUBLIC ${APP_LIB_HEADER} ${CMAKE_SOURCE_DIR}/src/shader)

add_library(${APP_LIB2_NAME} SHARED ${APP_LIB_SRC})
target_link_libraries(${APP_LIB2_NAME} PUBLIC glfw spdlog::spdlog ShaderLib)
target_include_directories(${APP_LIB2_NAME} PUBLIC ${APP_LIB_HEADER} ${CMAKE_SOURCE_DIR}/src/shader)