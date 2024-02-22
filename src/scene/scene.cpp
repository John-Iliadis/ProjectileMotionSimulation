//
// Created by Gianni on 20/02/2024.
//

#include "scene.hpp"


Scene::Scene(uint32_t width, uint32_t height)
    : m_fbo(GL_TEXTURE_2D_MULTISAMPLE, width, height)
    , m_intermediate_fbo(GL_TEXTURE_2D, width, height)
    //, m_camera(-1, 1, -1, 1)
   // , m_camera(1, 1)
    , m_graph_shader("../shaders/graph.vert", "../shaders/graph.frag")
    , m_width(width)
    , m_height(height)
{
    m_vertices = {
            0, 0,
            100, 200,
            200, 0
    };

//    m_vbo = VertexBufferStatic(m_vertices.data(), m_vertices.size());
//
//    VertexBufferLayout layout {
//            {0, 2, GL_FLOAT, GL_FALSE}
//    };
//
//    m_vao.attach_vertex_buffer(m_vbo, layout);
}

void Scene::update(double dt)
{

}

void Scene::render()
{
    pre_render();


    m_graph_shader.bind();
    m_graph_shader.set_mat4("u_view_projection_matrix", m_camera.get_view_projection_matrix());
    m_graph.render();
    m_graph_shader.unbind();

//    m_vao.bind();
//    m_graph_shader.bind();
//    m_graph_shader.set_mat4("u_view_projection_matrix", m_camera.get_view_projection_matrix());
//    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 2);
//    m_graph_shader.unbind();
//    m_vao.unbind();

    post_render();
}

void Scene::pre_render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("Scene");

    // todo: check if window is docked
    glm::vec<2, uint32_t> window_size {
            static_cast<uint32_t>(ImGui::GetContentRegionAvail().x),
            static_cast<uint32_t>(ImGui::GetContentRegionAvail().y)
    };

    if (m_width != window_size.x || m_height != window_size.y)
    {
        if (ImGui::IsWindowDocked())
        {
            resize_scene(window_size.x, window_size.y);
        }
        else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            resize_scene(window_size.x, window_size.y);
        }
    }

    m_fbo.bind();

    glClearColor(35 / 255.f, 45 / 255.f, 55 / 255.f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

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

    //float aspect_ratio = static_cast<float>(m_width) / static_cast<float>(m_height);

    m_camera.set_size(width, height);
//    m_camera.set_aspect_ratio(aspect_ratio);
//    m_camera.set_projection(-aspect_ratio, aspect_ratio, -1, 1, 0, 1);
}
