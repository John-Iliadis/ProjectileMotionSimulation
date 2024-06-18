//
// Created by Gianni on 20/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_SCENE_HPP
#define PROJECTILEMOTIONSIM_SCENE_HPP

#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include "../rendering/uniform_buffer_object_dynamic.hpp"
#include "../rendering/rendering_includes.hpp"
#include "../camera/orthographic_camera.hpp"
#include "../renderer2D/renderer2D_includes.hpp"
#include "graph.hpp"
#include "simulation.hpp"


// The scene consists of the window where the simulation happens.
// Everything is rendered to a framebuffer, which is then rendered to the 'scene' imgui dockable window.
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
    FrameBuffer m_fbo; // anti-aliased framebuffer
    FrameBuffer m_intermediate_fbo;
    UniformBufferObjectDynamic m_view_proj_ubo;
    OrthographicCamera m_camera;
    std::shared_ptr<Graph> m_graph;
    Simulation m_simulation;

    uint32_t m_width;
    uint32_t m_height;
};


#endif //PROJECTILEMOTIONSIM_SCENE_HPP
