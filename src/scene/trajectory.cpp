//
// Created by Gianni on 21/06/2024.
//

#include "trajectory.hpp"


static constexpr float LINE_LENGTH = 8.f;

Trajectory::Trajectory()
    : m_initial_position()
    , m_initial_velocity()
    , m_gravity()
    , m_meter_as_pixels()
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
    m_lines.clear();

    constexpr float TIME_STEP = 0.001f;

    glm::vec2 pos0 = m_initial_position;

    float time = TIME_STEP;
    bool line_spacing = false;

    while (time < simulation_time)
    {
        glm::vec2 pos1 = {x(time), y(time)};

        float hyp = std::hypot(pos1.x - pos0.x, pos1.y - pos0.y);

        if (hyp >= LINE_LENGTH)
        {
            if (!line_spacing)
            {
                m_lines.push_back(pos0);
                m_lines.push_back(pos1);
            }

            pos0 = pos1;
            line_spacing = !line_spacing;
        }

        time += TIME_STEP;
    }
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
