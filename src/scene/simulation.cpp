//
// Created by Gianni on 17/06/2024.
//

#include "simulation.hpp"


Simulation::Simulation(std::shared_ptr<Graph>& graph)
    : m_graph(graph)
    , m_state(State::INIT)
    , m_initial_velocity(2.f)
    , m_initial_angle(45)
    , m_initial_height(0.f)
    , m_velocity(m_initial_velocity * glm::cos(glm::radians(static_cast<float>(m_initial_angle))),
                 m_initial_velocity * glm::sin(glm::radians(static_cast<float>(m_initial_angle))))
    , m_position(m_graph->get_origin())
    , m_gravity(9.81f)
    , m_simulation_speed(SimulationSpeed::NORMAL)
    , m_duration(30)
    , m_vector_length(1)
    , m_show_velocity_vector(true)
    , m_show_velocity_vector_components(true)
    , m_show_trajectory(true)
    , m_velocity_vector(m_position,
                        m_initial_velocity,
                        m_initial_angle,
                        m_graph->get_meter_as_pixels(),
                        {1, 0, 0, 1})
    , m_velocity_vector_x_component(m_position,
                                    {m_velocity.x, 0},
                                    m_graph->get_meter_as_pixels(),
                                    {1, 0, 0, 1})
    , m_velocity_vector_y_component(m_position,
                                    {0, m_velocity.y},
                                    m_graph->get_meter_as_pixels(),
                                    {1, 0, 0, 1})
{
}

void Simulation::update(double dt)
{
    m_position = m_graph->get_origin(); // todo: remove this

    update_velocity();
    update_position();
    update_vectors();
}

void Simulation::update_velocity()
{
    switch (m_state)
    {
        case State::INIT:
            m_velocity.x = m_initial_velocity * glm::cos(glm::radians(static_cast<float>(m_initial_angle)));
            m_velocity.y = m_initial_velocity * glm::sin(glm::radians(static_cast<float>(m_initial_angle)));
            break;
        case State::RUNNING:
            break;
        case State::PAUSED:
            break;
    }
}

void Simulation::update_position()
{
    switch (m_state)
    {
        case State::INIT:
            m_position.y = m_graph->get_origin().y + m_initial_height * m_graph->get_meter_as_pixels();
            break;
        case State::RUNNING:
            break;
        case State::PAUSED:
            break;
    }
}

void Simulation::update_vectors()
{
    const auto meter_as_pixels = m_graph->get_meter_as_pixels();

    if (m_show_velocity_vector)
    {
        m_velocity_vector.set_velocity(m_velocity);
        m_velocity_vector.set_meter_as_pixels(meter_as_pixels);
        m_velocity_vector.set_position(m_position);
    }

    if (m_show_velocity_vector_components)
    {
        m_velocity_vector_x_component.set_velocity(m_velocity.x, 0);
        m_velocity_vector_x_component.set_meter_as_pixels(meter_as_pixels);
        m_velocity_vector_x_component.set_position(m_position);

        m_velocity_vector_y_component.set_velocity(0, m_velocity.y);
        m_velocity_vector_y_component.set_meter_as_pixels(meter_as_pixels);
        m_velocity_vector_y_component.set_position(m_position);
    }
}

void Simulation::render()
{
    render_vectors();
    control_panel();
    info_panel();
}

void Simulation::render_vectors()
{
    if (m_show_velocity_vector)
    {
        m_velocity_vector.render();
    }

    if (m_show_velocity_vector_components)
    {
        m_velocity_vector_x_component.render();
        m_velocity_vector_y_component.render();
    }
}

void Simulation::control_panel()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.f, 4.f));
    ImGui::Begin("Control Panel");

    constexpr ImVec2 BUTTON_SIZE(70, 20);

    { // Simulation controls
        ImGui::SeparatorText("Simulation Controls");

        ImGui::BeginDisabled(m_state != State::INIT);
        if (ImGui::Button("Start", BUTTON_SIZE))
            { m_state = State::RUNNING; }
        ImGui::EndDisabled();
        ImGui::SameLine(0.f, 4.f);

        ImGui::BeginDisabled(m_state != State::RUNNING);
        if (ImGui::Button("Pause", BUTTON_SIZE))
            { m_state = State::PAUSED; }
        ImGui::EndDisabled();
        ImGui::SameLine(0.f, 4.f);

        ImGui::BeginDisabled(m_state != State::PAUSED);
        if (ImGui::Button("Resume", BUTTON_SIZE))
            { m_state = State::RUNNING; }
        ImGui::EndDisabled();

        ImGui::BeginDisabled(m_state == State::INIT);
        if (ImGui::Button("Reset", BUTTON_SIZE))
            { m_state = State::INIT; reset(); }
        ImGui::EndDisabled();
    }

    { // Initial conditions
        ImGui::Spacing();
        ImGui::SeparatorText("Initial Conditions");

        ImGui::BeginDisabled(m_state != State::INIT);
        ImGui::SliderFloat("Velocity", &m_initial_velocity, 0, 100, "%.2f m/s");
        ImGui::SliderInt("Angle", &m_initial_angle, 0, 90, "%d deg");
        ImGui::SliderFloat("Height", &m_initial_height, 0, 100, "%.2f m");
        ImGui::EndDisabled();
    }

    { // Environmental factors
        ImGui::Spacing();
        ImGui::SeparatorText("Environmental Factors");
        ImGui::SliderFloat("Gravity", &m_gravity, 0, 30, "%.2f m/s^2");
    }

    static constexpr char const* simulation_speeds[Simulation::SimulationSpeed::COUNT]
    {
        "Very Slow",
        "Slow",
        "Normal",
        "Fast",
        "Very Fast"
    };

    { // Simulation settings
        ImGui::Spacing();
        ImGui::SeparatorText("Simulation Settings");
        ImGui::SliderInt("Speed",
                         reinterpret_cast<int*>(&m_simulation_speed),
                         0, SimulationSpeed::COUNT - 1,
                         simulation_speeds[m_simulation_speed]);
        ImGui::SliderInt("Duration", reinterpret_cast<int*>(&m_duration), 5, 100, "%d sec");
    }

    { // Visualization options
        ImGui::Spacing();
        ImGui::SeparatorText("Visualization Options");
        ImGui::SliderFloat("Vector Length", &m_vector_length, 0, 10, "%.2f");
        ImGui::Checkbox("Show Velocity Vector", &m_show_velocity_vector);
        ImGui::Checkbox("Show Velocity Vector Components", &m_show_velocity_vector_components);
        ImGui::Checkbox("Show Trajectory", &m_show_trajectory);
    }

    { // View controls
        ImGui::Spacing();
        ImGui::SeparatorText("View Controls");

        ImGui::BeginDisabled(m_graph->get_zoom_level() == 0);
        if (ImGui::Button("Zoom in", BUTTON_SIZE))
            { m_graph->zoom_in(); }
        ImGui::SameLine(0.f, 4.f);
        ImGui::EndDisabled();

        ImGui::BeginDisabled(m_graph->get_zoom_level() == m_graph->get_max_zoom_level());
        if (ImGui::Button("Zoom out", BUTTON_SIZE))
            { m_graph->zoom_out(); }
        ImGui::EndDisabled();
    }

    ImGui::End();
    ImGui::PopStyleVar(1);
}

void Simulation::info_panel()
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

void Simulation::reset()
{
    m_state = State::INIT;
    m_initial_velocity = 2.f;
    m_initial_angle = 45;
    m_initial_height = 0.f;
    m_position = m_graph->get_origin();
}
