//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_WINDOW_HPP
#define PROJECTILEMOTIONSIM_WINDOW_HPP

#include "../rendering/rendering_includes.hpp"
#include "../imgui/imgui_context.hpp"
#include "../scene/scene.hpp"
#include "window_base.hpp"


class Window : public WindowBase
{
public:
    Window(uint32_t width, uint32_t height);
    void run();

private:
    void update(double dt);
    void render();
    void update_frame_counter(double dt);

    static void key_callback(GLFWwindow* p_window, int key, int scancode, int action, int mods);

private:
    IMGUI_Context imgui_context;
    Scene m_scene;
    double fps_update_time;
    uint32_t m_frame_count;
};


#endif //PROJECTILEMOTIONSIM_WINDOW_HPP
