//
// Created by Gianni on 19/02/2024.
//

#include "window.hpp"
#include <thread>


Window::Window(uint32_t width, uint32_t height)
    : WindowBase(width, height)
    , imgui_context(m_window)
    , m_scene(width, height)
{
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, key_callback);
}

// main loop of application
void Window::run()
{
    constexpr float MAX_FPS = 120.f;
    constexpr float FRAME_TIME = 1.f / MAX_FPS;

    float last_time = glfwGetTime();
    float last_update_time = glfwGetTime();
    float accumulated_time = 0.f;

    while (!glfwWindowShouldClose(m_window))
    {
        float current_time = glfwGetTime();
        float dt = current_time - last_time;
        last_time = current_time;
        accumulated_time += dt;

        glfwPollEvents();

        if (accumulated_time >= FRAME_TIME)
        {
            float update_dt = current_time - last_update_time;
            last_update_time = current_time;
            accumulated_time = 0.f;

            update(update_dt);
            render();
        }
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

void Window::update(float dt)
{
    imgui_context.begin();
    m_scene.update(dt);
}

void Window::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_scene.render();

    imgui_context.end();
    glfwSwapBuffers(m_window);
}
