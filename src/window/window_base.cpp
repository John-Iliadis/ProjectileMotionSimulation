//
// Created by Gianni on 19/02/2024.
//

#include "window_base.hpp"

WindowBase::WindowBase(uint32_t width, uint32_t height)
{
    if (glfwInit() == GLFW_FALSE)
    {
        throw std::runtime_error("WindowBase::WindowBase: Failed to initialize glfw\n");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    glm::vec<2, int> window_size {
        static_cast<int>(width),
        static_cast<int>(height)
    };

    const char* window_title = "Projectile Motion Simulation";

    m_window = glfwCreateWindow(window_size.x, window_size.y, window_title, nullptr, nullptr);

    if (!m_window)
    {
        throw std::runtime_error("WindowBase::WindowBase: Failed to create window\n");
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("WindowBase::WindowBase: Failed to load glad\n");
    }

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, nullptr);
}

WindowBase::~WindowBase()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
