//
// Created by Gianni on 17/06/2024.
//

#include "simulation.hpp"


#define IMGUI_TEXT(str, char_count, offset, format, value)\
    {char buffer[char_count];                             \
    memcpy(buffer, str, strlen(str));                     \
    sprintf(buffer + offset, format, value);              \
    ImGui::Text(buffer);}                                 \

Simulation::Simulation(std::shared_ptr<Graph>& graph)
    : m_graph(graph)
    , m_state(State::INIT)
    , m_origin(m_graph->get_origin())
    , m_meter_as_pixels(m_graph->get_meter_as_pixels())
    , m_initial_velocity(25.f)
    , m_initial_angle(45.f)
    , m_initial_height(0.f)
    , m_initial_velocity_components(m_initial_velocity * glm::cos(glm::radians(m_initial_angle)),
                                    m_initial_velocity * glm::sin(glm::radians(m_initial_angle)))
    , m_velocity(m_initial_velocity_components)
    , m_position(m_origin)
    , m_simulation_time()
    , m_gravity(9.81f)
    , m_simulation_speed(SimulationSpeed::NORMAL)
    , m_duration(10.f)
    , m_vector_magnification(1.f)
    , m_show_velocity_vector(true)
    , m_show_velocity_vector_components(true)
    , m_show_trajectory(true)
    , m_projectile({14, 14}, m_position)
    , m_velocity_vector(m_position, m_initial_velocity, m_initial_angle, m_meter_as_pixels)
    , m_velocity_vector_x_component(m_position, {m_velocity.x, 0}, m_meter_as_pixels)
    , m_velocity_vector_y_component(m_position, {0, m_velocity.y}, m_meter_as_pixels)
{
    m_graph->zoom_out(3);
    m_velocity_vector.set_color_rgb(255, 0, 0);
    m_velocity_vector_x_component.set_color_rgb(0, 0, 255);
    m_velocity_vector_y_component.set_color_rgb(0, 0, 255);
}

void Simulation::update(float dt)
{
    m_origin = m_graph->get_origin();
    m_meter_as_pixels = m_graph->get_meter_as_pixels();

    update_simulation_time(dt);
    update_position();
    update_velocity();
    update_vectors();
    update_projectile();
    update_trajectory();
}

void Simulation::update_simulation_time(float dt)
{
    if (m_state == State::RUNNING)
    {
        m_simulation_time += dt * (0.25 * std::pow(2, static_cast<int>(m_simulation_speed)));

        if (m_simulation_time >= m_duration)
        {
            m_simulation_time = m_duration;
            m_state = State::FINISHED;
        }
    }
}

void Simulation::update_velocity()
{
    switch (m_state)
    {
        case State::INIT:
        {
            m_initial_velocity_components.x = m_initial_velocity * glm::cos(glm::radians(m_initial_angle));
            m_initial_velocity_components.y = m_initial_velocity * glm::sin(glm::radians(m_initial_angle));
            m_velocity = m_initial_velocity_components;
            break;
        }

        case State::RUNNING:
        {
            m_velocity.y = m_initial_velocity_components.y - m_gravity * m_simulation_time;
            break;
        }

        case State::PAUSED:
            break;

        case State::FINISHED:
            break;
    }
}

void Simulation::update_position()
{
    switch (m_state)
    {
        case State::INIT:
        {
            m_position.x = m_origin.x;
            m_position.y = m_origin.y + m_initial_height * m_meter_as_pixels;
            break;
        }

        case State::PAUSED:
        case State::FINISHED:
        case State::RUNNING:
        {
            m_position.x = m_origin.x + m_initial_velocity_components.x * m_simulation_time * m_meter_as_pixels;

            const float yo = m_origin.y + m_initial_height * m_meter_as_pixels;
            const float y_offset = m_initial_velocity_components.y * m_simulation_time - m_gravity * std::pow(m_simulation_time, 2) / 2.f;
            m_position.y = yo + y_offset * m_meter_as_pixels;
            break;
        }
    }
}

void Simulation::update_vectors()
{
    if (m_show_velocity_vector)
    {
        m_velocity_vector.set_magnitude(m_velocity);
        m_velocity_vector.set_meter_as_pixels(m_meter_as_pixels);
        m_velocity_vector.set_position(m_position);
    }

    if (m_show_velocity_vector_components)
    {
        m_velocity_vector_x_component.set_magnitude(m_velocity.x, 0);
        m_velocity_vector_x_component.set_meter_as_pixels(m_meter_as_pixels);
        m_velocity_vector_x_component.set_position(m_position);

        m_velocity_vector_y_component.set_magnitude(0, m_velocity.y);
        m_velocity_vector_y_component.set_meter_as_pixels(m_meter_as_pixels);
        m_velocity_vector_y_component.set_position(m_position);
    }
}

void Simulation::update_projectile()
{
    m_projectile.set_position(m_position);
}

void Simulation::update_trajectory()
{
    if (!m_show_trajectory)
        return;

    m_trajectory.set_meter_as_pixels(m_meter_as_pixels);
    m_trajectory.set_initial_position(m_graph->get_origin());

    switch (m_state)
    {
        case State::INIT:
        {
            m_trajectory.set_gravity(m_gravity);
            m_trajectory.set_initial_velocity({m_initial_velocity * glm::cos(glm::radians(m_initial_angle)),
                                               m_initial_velocity * glm::sin(glm::radians(m_initial_angle))});
            break;
        }

        case State::RUNNING:
        case State::PAUSED:
        case State::FINISHED:
        {
            m_trajectory.update(m_simulation_time);
            break;
        }
    }
}

void Simulation::render()
{
    render_vectors();
    m_projectile.render();
    m_show_trajectory? m_trajectory.render() : void(0);
    control_panel();
}

void Simulation::render_vectors()
{
    if (m_show_velocity_vector_components)
    {
        m_velocity_vector_x_component.render();
        m_velocity_vector_y_component.render();
    }

    if (m_show_velocity_vector)
    {
        m_velocity_vector.render();
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
            { m_state = State::INIT; reset(); m_trajectory.clear(); }
        ImGui::EndDisabled();
    }

    { // Initial conditions
        ImGui::Spacing();
        ImGui::SeparatorText("Initial Conditions");

        ImGui::BeginDisabled(m_state != State::INIT);
        ImGui::SliderFloat("Velocity", &m_initial_velocity, 0, 100, "%.2f m/s");
        ImGui::SliderFloat("Angle", &m_initial_angle, 0.f, 90.f, "%.0f deg");
        ImGui::SliderFloat("Height", &m_initial_height, 0, 100, "%.2f m");
        ImGui::SliderFloat("Gravity", &m_gravity, 0, 30, "%.2f m/s^2");
        ImGui::EndDisabled();
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
        ImGui::BeginDisabled(m_state != State::INIT);
        ImGui::SliderFloat("Duration", &m_duration, 5, 100, "%.0f sec");
        ImGui::EndDisabled();
    }

    { // Visualization options
        ImGui::Spacing();
        ImGui::SeparatorText("Visualization Options");

        if (ImGui::SliderFloat("Vector Size", &m_vector_magnification, 0.1f, 5, "%.2f"))
        {
            m_velocity_vector.set_magnification(m_vector_magnification);
            m_velocity_vector_x_component.set_magnification(m_vector_magnification);
            m_velocity_vector_y_component.set_magnification(m_vector_magnification);
        }

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

    { // Progress bar
        ImGui::Spacing();
        ImGui::SeparatorText("Progress");
        ImGui::ProgressBar(m_simulation_time / m_duration);
    }

    { // Stats
        ImGui::Spacing();
        ImGui::SeparatorText("Info");
        IMGUI_TEXT("Time: ", 50, 6, "%.2f sec", m_simulation_time)
        ImGui::Spacing();
        IMGUI_TEXT("X Position: ", 50, 12, "%.2f m", (m_position.x - m_origin.x) / m_meter_as_pixels)
        IMGUI_TEXT("Y Position: ", 50, 12, "%.2f m", (m_position.y - m_origin.y) / m_meter_as_pixels)
        ImGui::Spacing();
        IMGUI_TEXT("Velocity: ", 50, 10, "%.2f m/s", std::hypot(m_velocity.x, m_velocity.y))
        IMGUI_TEXT("Velocity.x: ", 50, 12, "%.2f m/s", std::abs(m_velocity.x) > 1e-2f? m_velocity.x : 0.f)
        IMGUI_TEXT("Velocity.y: ", 50, 12, "%.2f m/s", m_velocity.y)
    }

    ImGui::End();
    ImGui::PopStyleVar(1);
}

void Simulation::reset()
{
    m_state = State::INIT;
    m_position = m_origin;
    m_simulation_time = 0;
}
