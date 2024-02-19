//
// Created by Gianni on 20/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_IMGUI_CONTEXT_HPP
#define PROJECTILEMOTIONSIM_IMGUI_CONTEXT_HPP

#include "glfw/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


class IMGUI_Context
{
public:
    IMGUI_Context(GLFWwindow* window);
    ~IMGUI_Context();

    void begin();
    void end();
};


#endif //PROJECTILEMOTIONSIM_IMGUI_CONTEXT_HPP
