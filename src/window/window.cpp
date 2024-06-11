//
// Created by Gianni on 19/02/2024.
//

#include "window.hpp"


Window::Window(uint32_t width, uint32_t height)
    : WindowBase(width, height)
    , imgui_context(m_window)
    , m_scene(width, height)
    , fps_update_time()
    , m_frame_count()
{
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, key_callback);
}

// main loop of application
void Window::run()
{
    double last_time {};

    while (!glfwWindowShouldClose(m_window))
    {
        double current_time = glfwGetTime();
        double dt = current_time - last_time;
        last_time = current_time;

        glfwPollEvents();
        update(dt);
        update_frame_counter(dt);
        render();
    }
}

// callback that handles key events
void Window::key_callback(GLFWwindow *p_window, int key, int scancode, int action, int mods)
{
    auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window->m_window, GLFW_TRUE);
    }
}

void Window::update(double dt)
{
    m_scene.update(dt);
}

void Window::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    imgui_context.begin();

    ImGui::ShowDemoWindow();
    m_scene.render();

    imgui_context.end();
    glfwSwapBuffers(m_window);
}

// fps counter
void Window::update_frame_counter(double dt)
{
    fps_update_time += dt;
    ++m_frame_count;

    if (fps_update_time > 1)
    {
        std::cout << "Fps: " << m_frame_count << '\n';
        m_frame_count = 0;
        fps_update_time -= 1;
    }
}
