//
// Created by Gianni on 20/02/2024.
//

#include "scene.hpp"

#include "../utilities/utils.hpp"


Scene::Scene(uint32_t width, uint32_t height)
    : m_fbo(GL_TEXTURE_2D_MULTISAMPLE, width, height)
    , m_intermediate_fbo(GL_TEXTURE_2D, width, height)
    , m_graph_shader("../shaders/graph.vert", "../shaders/graph.frag")
    , m_width(width)
    , m_height(height)
    , m_arial_48("../assets/fonts/arial.ttf", 48)
    , text(&m_arial_48, "HELLO WORLD", {0, 0})
{
}

void Scene::update(double dt)
{

}

void Scene::render()
{
    pre_render();
    TextRenderer::pre_render(m_camera);

    m_graph_shader.bind();
    m_graph_shader.set_mat4("u_view_projection_matrix", m_camera.get_view_projection_matrix());
    m_graph.render();
    m_graph_shader.unbind();

    TextRenderer::draw_text(text);

    TextRenderer::render();
    post_render();
}

// The pre-render function:
// - handles resizing
// - binds the framebuffer
// - clears the color buffer of the framebuffer
void Scene::pre_render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // no window padding
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0); // no border
    ImGui::Begin("Scene");

    // todo: check if window is docked
    glm::uvec2 window_size
    {
        static_cast<uint32_t>(ImGui::GetContentRegionAvail().x),
        static_cast<uint32_t>(ImGui::GetContentRegionAvail().y)
    };

    if (m_width != window_size.x || m_height != window_size.y)
    {
        if (ImGui::IsWindowDocked() || ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            resize_scene(window_size.x, window_size.y);
        }
    }

    m_fbo.bind();

    glClearColor(35 / 255.f, 45 / 255.f, 55 / 255.f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

// The post render:
// - transfers the aa-framebuffers data to a texture
// - draws the texture to the 'scene' imgui window
void Scene::post_render()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo.get_framebuffer_id());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_intermediate_fbo.get_framebuffer_id());
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    m_fbo.unbind();

    uint32_t render_buffer_texture_id = m_intermediate_fbo.get_texture_id();

    ImGui::Image(reinterpret_cast<void*>(render_buffer_texture_id), ImVec2(m_width, m_height), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar(2);
}

void Scene::resize_scene(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;

    m_graph.create(m_width, m_height);

    m_fbo.resize(m_width, m_height);
    m_intermediate_fbo.resize(m_width, m_height);

    m_camera.set_size(width, height);
}
