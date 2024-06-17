//
// Created by Gianni on 20/02/2024.
//

#include "scene.hpp"


Scene::Scene(uint32_t width, uint32_t height)
    : m_fbo(GL_TEXTURE_2D_MULTISAMPLE, width, height)
    , m_intermediate_fbo(GL_TEXTURE_2D, width, height)
    , m_width(width)
    , m_height(height)
{
}

void Scene::update(double dt)
{

}

void Scene::render()
{
    pre_render();
    renderer2D::pre_render(m_camera);

    m_graph.render();
    control_panel();
    info_panel();

    renderer2D::render();
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

void Scene::control_panel()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.f, 4.f));
    ImGui::Begin("Control Panel");

    constexpr ImVec2 BUTTON_SIZE(70, 20);
    ImGui::SeparatorText("Simulation Controls");
    ImGui::Button("Start", BUTTON_SIZE); ImGui::SameLine(0.f, 4.f);
    ImGui::Button("Pause", BUTTON_SIZE); ImGui::SameLine(0.f, 4.f);
    ImGui::Button("Resume", BUTTON_SIZE);
    ImGui::Button("Reset", BUTTON_SIZE);

    float x = 5;
    bool y = x;
    int z = 30;

    ImGui::Spacing();
    ImGui::SeparatorText("Initial Conditions");
    ImGui::SliderFloat("Velocity [m/s]", &x, 0, 100, "%.2f");
    ImGui::SliderFloat("Angle [deg]", &x, 0, 90, "%.2f");
    ImGui::SliderFloat("Height [m]", &x, 0, 100, "%.2f");

    ImGui::Spacing();
    ImGui::SeparatorText("Environmental Factors");
    ImGui::SliderFloat("Gravity [m/s^2]", &x, 0, 30, "%.2f");

    static int f = 0;
    constexpr char const* speeds[] {"Very Slow", "Slow", "Normal", "Fast", "Very Fast"};

    ImGui::Spacing();
    ImGui::SeparatorText("Simulation Settings");

    ImGui::SliderInt("Simulation Speed", &f, 0, 4, speeds[f]);
    ImGui::SetNextItemWidth(120);
    ImGui::InputInt("Simulation Time [sec]", &z);

    ImGui::Spacing();
    ImGui::SeparatorText("Visualization Options");
    ImGui::SliderFloat("Vector Length", &x, 0, 100);
    ImGui::Checkbox("Show Velocity Vector", &y);
    ImGui::Checkbox("Show Velocity Vector Components", &y);
    ImGui::Checkbox("Show Trajectory", &y);

    ImGui::Spacing();
    ImGui::SeparatorText("View Controls");
    ImGui::Button("Zoom in", BUTTON_SIZE); ImGui::SameLine(0.f, 4.f);
    ImGui::Button("Zoom out", BUTTON_SIZE);

    ImGui::End();
    ImGui::PopStyleVar(1);
}

void Scene::info_panel()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.f, 4.f));
    ImGui::Begin("Info Panel");

    static float x = 0;

    ImGui::SeparatorText("Initial Conditions");
    ImGui::SliderFloat("Velocity [m/s]", &x, 0, 100, "%.2f");
    ImGui::SliderFloat("Angle [deg]", &x, 0, 90, "%.2f");
    ImGui::SliderFloat("Height [m]", &x, 0, 100, "%.2f");

    ImGui::End();
    ImGui::PopStyleVar(1);
}
