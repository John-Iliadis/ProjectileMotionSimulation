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
        src/rendering/shader.cpp
        src/rendering/shader.hpp
        src/rendering/vertex_buffer_static.cpp
        src/rendering/vertex_buffer_static.hpp
        src/rendering/index_buffer_static.cpp
        src/rendering/index_buffer_static.hpp
        src/rendering/vertex_buffer_layout.cpp
        src/rendering/vertex_buffer_layout.hpp
        src/rendering/vertex_array.cpp
        src/rendering/vertex_array.hpp
        src/rendering/frame_buffer.cpp
        src/rendering/frame_buffer.hpp
        src/rendering/rendering_includes.hpp
        src/rendering/vertex_buffer_dynamic.cpp
        src/rendering/vertex_buffer_dynamic.hpp
        src/rendering/index_buffer_dynamic.cpp
        src/rendering/index_buffer_dynamic.hpp
        src/rendering/vertex_buffer.hpp
        src/rendering/index_buffer.hpp
        src/imgui/imgui_context.cpp
        src/imgui/imgui_context.hpp
        src/camera/orthographic_camera.cpp
        src/camera/orthographic_camera.hpp
        src/scene/scene.cpp
        src/scene/scene.hpp
        src/scene/graph.cpp
        src/scene/graph.hpp
        src/renderer2D/font_atlas.cpp
        src/renderer2D/font_atlas.hpp
        src/renderer2D/renderer2D.cpp
        src/renderer2D/renderer2D.hpp
        src/renderer2D/text.hpp
        src/renderer2D/renderer2D_includes.hpp
        src/scene/vector.cpp
        src/scene/vector.hpp
        src/scene/simulation.cpp
        src/scene/simulation.hpp
        src/renderer2D/renderer2D_constants.hpp
        src/rendering/uniform_buffer_object_dynamic.cpp
        src/rendering/uniform_buffer_object_dynamic.hpp)

find_package(OpenGL REQUIRED)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

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
