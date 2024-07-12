//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_WINDOW_HPP
#define PROJECTILEMOTIONSIM_WINDOW_HPP

#include "../rendering/rendering_includes.hpp"
#include "../imgui/imgui_context.hpp"
#include "../simulation/scene.hpp"
#include "window_base.hpp"


class Window : public WindowBase
{
public:
    Window(uint32_t width, uint32_t height);
    void run();

private:
    void update(float dt);
    void render();

    static void key_callback(GLFWwindow* p_window, int key, int scancode, int action, int mods);

private:
    IMGUI_Context imgui_context;
    Scene m_scene;
};


#endif //PROJECTILEMOTIONSIM_WINDOW_HPP
