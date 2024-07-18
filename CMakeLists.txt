cmake_minimum_required(VERSION 3.27)
project(ProjectileMotionSim)

set(CMAKE_CXX_STANDARD 23)

set(GLAD_DIR ${CMAKE_CURRENT_LIST_DIR}/dependencies/glad)
set(GLFW_DIR ${CMAKE_CURRENT_LIST_DIR}/dependencies/glfw)
set(GLM_DIR ${CMAKE_CURRENT_LIST_DIR}/dependencies/glm)
set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/dependencies/imgui)
set(FREE_TYPE_DIR ${CMAKE_CURRENT_LIST_DIR}/dependencies/free_type)
set(STB_DIR ${CMAKE_CURRENT_LIST_DIR}/dependencies/stb)

file(GLOB_RECURSE GLAD_SRC ${GLAD_DIR}/*.c)
file(GLOB_RECURSE GLAD_HEADERS ${GLAD_DIR}/*.h)
file(GLOB_RECURSE IMGUI_SRC ${IMGUI_DIR}/*.cpp)
file(GLOB_RECURSE IMGUI_HEADERS ${IMGUI_DIR}/*.h)

add_executable(${PROJECT_NAME}
        ${GLAD_SRC}
        ${GLAD_HEADERS}
        ${IMGUI_SRC}
        ${IMGUI_HEADERS}
        src/main.cpp
        src/utilities/gl_debug_message_callback.cpp
        src/utilities/gl_debug_message_callback.hpp
        src/utilities/utils.cpp
        src/utilities/utils.hpp
        src/window/window.cpp
        src/window/window.hpp
        src/window/window_base.cpp
        src/window/window_base.hpp
        src/opengl/shader.cpp
        src/opengl/shader.hpp
        src/opengl/vertex_buffer_static.cpp
        src/opengl/vertex_buffer_static.hpp
        src/opengl/index_buffer_static.cpp
        src/opengl/index_buffer_static.hpp
        src/opengl/vertex_buffer_layout.cpp
        src/opengl/vertex_buffer_layout.hpp
        src/opengl/vertex_array.cpp
        src/opengl/vertex_array.hpp
        src/opengl/frame_buffer.cpp
        src/opengl/frame_buffer.hpp
        src/opengl/includes.hpp
        src/opengl/vertex_buffer_dynamic.cpp
        src/opengl/vertex_buffer_dynamic.hpp
        src/opengl/index_buffer_dynamic.cpp
        src/opengl/index_buffer_dynamic.hpp
        src/opengl/vertex_buffer.hpp
        src/opengl/index_buffer.hpp
        src/imgui/imgui_context.cpp
        src/imgui/imgui_context.hpp
        src/camera/orthographic_camera.cpp
        src/camera/orthographic_camera.hpp
        src/simulation/scene.cpp
        src/simulation/scene.hpp
        src/simulation/graph.cpp
        src/simulation/graph.hpp
        src/renderer2D/font_atlas.cpp
        src/renderer2D/font_atlas.hpp
        src/renderer2D/renderer2D.cpp
        src/renderer2D/renderer2D.hpp
        src/renderer2D/text.hpp
        src/renderer2D/renderer2D_includes.hpp
        src/simulation/vector.cpp
        src/simulation/vector.hpp
        src/simulation/simulation.cpp
        src/simulation/simulation.hpp
        src/renderer2D/renderer2D_constants.hpp
        src/opengl/uniform_buffer_object_dynamic.cpp
        src/opengl/uniform_buffer_object_dynamic.hpp
        src/opengl/texture.cpp
        src/opengl/texture.hpp
        src/simulation/projectile.cpp
        src/simulation/projectile.hpp
        src/simulation/trajectory.cpp
        src/simulation/trajectory.hpp)

find_package(OpenGL REQUIRED)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        GLFW_INCLUDE_NONE
        STB_IMAGE_WRITE_IMPLEMENTATION
        STB_IMAGE_IMPLEMENTATION)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${GLAD_DIR}/include
        ${GLFW_DIR}/include
        ${GLM_DIR}/include
        ${IMGUI_DIR}/include
        ${FREE_TYPE_DIR}/include
        ${STB_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${OPENGL_LIBRARIES}
        ${GLFW_DIR}/lib/libglfw3.a
        ${FREE_TYPE_DIR}/lib/libfreetype.a)

target_link_options(${PROJECT_NAME} PRIVATE -static "-Wl,--allow-multiple-definition")
