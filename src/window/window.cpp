//
// Created by Gianni on 19/02/2024.
//

#include "window.hpp"


Window::Window(uint32_t width, uint32_t height)
    : WindowBase(width, height)
    , imgui_context(m_window)
    , m_scene(width, height)
{
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, key_callback);
}

void Window::run()
{
    while (!glfwWindowShouldClose(m_window))
    {

        glClear(GL_COLOR_BUFFER_BIT);
        imgui_context.begin();

        ImGui::ShowDemoWindow();
        m_scene.render();

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
