//
// Created by Gianni on 19/02/2024.
//

#include "window.hpp"
#include <thread>


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
    constexpr float max_fps = 240.f;
    constexpr float max_frame_time = 1.f / max_fps;
    float last_time = glfwGetTime();

    while (!glfwWindowShouldClose(m_window))
    {
        float current_time = glfwGetTime();
        float dt = current_time - last_time;
        last_time = current_time;

        update_frame_counter(dt);
        glfwPollEvents();
        update(dt);
        render();

        float frame_time = glfwGetTime() - current_time;
        if (frame_time < max_frame_time)
        {
            std::this_thread::sleep_for(std::chrono::duration<float>(max_frame_time - frame_time));
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
    m_scene.update(dt);
}

void Window::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    imgui_context.begin();

    m_scene.render();

    imgui_context.end();
    glfwSwapBuffers(m_window);
}

// fps counter
void Window::update_frame_counter(float dt)
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
