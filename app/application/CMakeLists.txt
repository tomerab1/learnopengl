set(LIB_NAME "AppLib")

if (DEFINED NEW_LIB_NAME)
    set(LIB_NAME ${NEW_LIB_NAME})
endif()


set(APP_LIB_SRC "application.cpp" "${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c" )
set(APP_LIB_HEADER "application.h" 
"${PROJECT_SOURCE_DIR}/vendor/glad/include" 
"${CMAKE_BINARY_DIR}/configured_files/include")

add_library(${LIB_NAME} SHARED ${APP_LIB_SRC})
target_link_libraries(${LIB_NAME} PUBLIC glfw spdlog::spdlog ShaderLib GlfwBackendLib)
target_include_directories(${LIB_NAME} PUBLIC ${APP_LIB_HEADER} ${CMAKE_SOURCE_DIR}/src/shader ${CMAKE_SOURCE_DIR}/src/backend)