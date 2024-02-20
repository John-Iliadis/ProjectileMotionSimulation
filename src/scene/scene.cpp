//
// Created by Gianni on 20/02/2024.
//

#include "scene.hpp"


Scene::Scene(uint32_t width, uint32_t height)
    : m_frame_buffer(width, height)
    , m_shader("../shaders/vert.vert", "../shaders/frag.frag")
    , m_width(width)
    , m_height(height)
{
    vertices = {
            -0.5, -0.5, 1, 0, 0,
            0, 0.5, 0, 1, 0,
            0.5, -0.5, 0, 0, 1
    };

    vbo = VertexBufferStatic(vertices.data(), vertices.size());

    VertexBufferLayout layout {
            {0, 2, GL_FLOAT, GL_FALSE},
            {1, 3, GL_FLOAT, GL_FALSE}
    };

    vao.attach_vertex_buffer(vbo, layout);
    vao.bind();

    m_shader.bind();
}

void Scene::resize_scene(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;

    m_frame_buffer.resize(m_width, m_height);
}

void Scene::render()
{
    pre_render();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    post_render();
}

void Scene::pre_render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("Scene");

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

    m_frame_buffer.bind();

    glClearColor(35 / 255.f, 45 / 255.f, 55 / 255.f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Scene::post_render()
{
    m_frame_buffer.unbind();

    uint32_t render_buffer_texture_id = m_frame_buffer.get_texture_id();

    ImGui::Image(reinterpret_cast<void*>(render_buffer_texture_id), ImVec2(m_width, m_height), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar(2);
}