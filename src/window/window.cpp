//
// Created by Gianni on 19/02/2024.
//

#include "window.hpp"

Window::Window(uint32_t width, uint32_t height)
    : WindowBase(width, height)
    , imgui_context(m_window)
{
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, key_callback);
}

void Window::run()
{
    float triangle_array[] {
        -0.5, -0.5, 1, 0, 0,
        0, 0.5, 0, 1, 0,
        0.5, -0.5, 0, 0, 1
    };

    VertexArray vao;

    VertexBufferStatic vbo = VertexBufferStatic(triangle_array, sizeof(triangle_array) / sizeof(float));
    VertexBufferLayout layout {
            {0, 2, GL_FLOAT, GL_FALSE},
            {1, 3, GL_FLOAT, GL_FALSE}
    };

    vao.attach_vertex_buffer(vbo, layout);
    vao.bind();

    Shader shader = Shader("../shaders/vert.vert", "../shaders/frag.frag");
    shader.bind();

    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        imgui_context.begin();

        ImGui::ShowDemoWindow();

        glDrawArrays(GL_TRIANGLES, 0, 3);


        imgui_context.end();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Window::key_callback(GLFWwindow *p_window, int key, int scancode, int action, int mods)
{
    auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window->m_window, GLFW_TRUE);
    }
}
