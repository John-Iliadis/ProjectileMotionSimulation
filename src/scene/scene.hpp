//
// Created by Gianni on 20/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_SCENE_HPP
#define PROJECTILEMOTIONSIM_SCENE_HPP

#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include "../rendering/rendering_includes.hpp"
#include "../camera/orthographic_camera.hpp"
#include "../text_rendering/font_atlas.hpp"
#include "graph.hpp"


class Scene
{
public:
    Scene(uint32_t width, uint32_t height);

    void update(double dt);
    void render();

private:
    void pre_render();
    void post_render();
    void resize_scene(uint32_t width, uint32_t height);

private:
    FrameBuffer m_fbo;
    FrameBuffer m_intermediate_fbo;
    OrthographicCamera m_camera;
    Shader m_graph_shader;
    Graph m_graph;

    uint32_t m_width;
    uint32_t m_height;

    FontAtlas m_arial_48;
};


#endif //PROJECTILEMOTIONSIM_SCENE_HPP
