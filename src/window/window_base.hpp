//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_WINDOW_BASE_HPP
#define PROJECTILEMOTIONSIM_WINDOW_BASE_HPP

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include "../utilities/gl_debug_message_callback.hpp"


class WindowBase
{
public:
    WindowBase(uint32_t width, uint32_t height);
    virtual ~WindowBase();

protected:
    GLFWwindow* m_window;
};


#endif //PROJECTILEMOTIONSIM_WINDOW_BASE_HPP
