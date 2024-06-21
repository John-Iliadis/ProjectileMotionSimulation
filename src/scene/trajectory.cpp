//
// Created by Gianni on 21/06/2024.
//

#include "trajectory.hpp"


static constexpr float LINE_LENGTH = 1.f;

Trajectory::Trajectory()
    : m_initial_position()
    , m_initial_velocity()
    , m_gravity()
    , m_color(1.f)
{
}

Trajectory::Trajectory(const glm::vec2 &initial_position,
                       const glm::vec2 &initial_velocity,
                       const float gravity,
                       const float meter_as_pixels,
                       const glm::vec4& color)
   : m_initial_position(initial_position)
   , m_initial_velocity(initial_velocity)
   , m_gravity(gravity)
   , m_meter_as_pixels(meter_as_pixels)
   , m_color(color)
{
}

void Trajectory::update(const float simulation_time)
{
    clear();

    glm::vec2 velocity = m_initial_velocity;
    glm::vec2 last_position = m_initial_position;
    float time = 0;
    bool line_spacing = false;

    while (time < simulation_time)
    {
        // polynomial g^2 * t^4 - Vy * g * t^3 + (Vx^2 + Vy^2)t^2 - L^2 = 0
        // solve for t
        std::array<float, 5> poly4
        {
            std::pow(m_gravity, 2.f) / 4.f,
            - velocity.y * m_gravity,
            std::pow(velocity.x, 2.f) + std::pow(velocity.y, 2.f),
            0,
            - std::pow(LINE_LENGTH, 2.f)
        };

        float t = utils::poly4_root(poly4);

        time = time + t < simulation_time? time + t : simulation_time;

        glm::vec2 new_pos = {x(time), y(time)};

        if (!line_spacing)
        {
            m_lines.push_back(glm::vec2(last_position));
            m_lines.push_back(new_pos);
        }

        line_spacing = !line_spacing;
        last_position = new_pos;
        velocity.y = m_initial_velocity.y - m_gravity * time; // Vy = Voy - gt
    }

    // todo: break when going out of bounds
}

void Trajectory::render()
{
    assert(m_lines.size() % 2 == 0);

    for (size_t i = 0; i < m_lines.size(); i += 2)
    {
        renderer2D::draw_line(m_lines.at(i), m_lines.at(i + 1), m_color);
    }
}

void Trajectory::set_initial_position(const glm::vec2 &pos)
{
    m_initial_position = pos;
}

void Trajectory::set_initial_velocity(const glm::vec2 &vel)
{
    m_initial_velocity = vel;
}

void Trajectory::set_gravity(const float gravity)
{
    m_gravity = gravity;
}

void Trajectory::set_color(const glm::vec4 &color)
{
    m_color = color;
}

float Trajectory::x(const float time)
{
    return m_initial_position.x + m_initial_velocity.x * time * m_meter_as_pixels;
}

float Trajectory::y(const float time)
{
    return m_initial_position.y + (m_initial_velocity.y * time - m_gravity * std::pow(time, 2) / 2.f) * m_meter_as_pixels;
}

void Trajectory::set_meter_as_pixels(const float meter_as_pixels)
{
    m_meter_as_pixels = meter_as_pixels;
}

void Trajectory::clear()
{
    m_lines.clear();
}
