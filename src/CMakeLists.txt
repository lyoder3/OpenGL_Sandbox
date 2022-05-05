cmake_minimum_required(VERSION 3.16.3)

project(ORVIS)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++17")
set (OpenGL_GL_PREFERENCE "GLVND")

set (source_dir "${PROJECT_SOURCE_DIR}/src/")

add_executable(${PROJECT_NAME} 
    main.cpp 
    Renderer.cpp 
    VertexBuffer.cpp 
    IndexBuffer.cpp 
    VertexBufferLayout.h 
    VertexArray.cpp 
    Shader.cpp
    GLErrorHandler.cpp
    Color.cpp)

add_subdirectory(../external/glfw ./external/glfw)
add_subdirectory(../external/glew/build/cmake ./external/glew)
add_subdirectory(../external/stb_image ./external/stb_image)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glew)
target_link_libraries(${PROJECT_NAME} STB_Image)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
