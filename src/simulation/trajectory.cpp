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
    , m_duration()
    , m_simulation_time()
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
   , m_duration()
   , m_simulation_time()
{
}

void Trajectory::update(float simulation_time)
{
    m_simulation_time = simulation_time;
}

void Trajectory::render()
{
    assert(m_lines.size() % 2 == 0);

    for (size_t i = 0; i < m_lines.size(); i += 2)
    {
        if (m_lines.at(i + 1).time < m_simulation_time)
        {
            renderer2D::draw_line(m_lines.at(i).x,
                                  m_lines.at(i).y,
                                  m_lines.at(i + 1).x,
                                  m_lines.at(i + 1).y);
        }
    }
}

void Trajectory::make_lines()
{
    m_lines.clear();

    bool line_spacing = false;
    float time = 0;
    float next_time = t(m_initial_velocity.x, m_initial_velocity.y);

    while (time < m_duration)
    {
        if (!line_spacing)
        {
            m_lines.emplace_back(x(time), y(time), time);
            m_lines.emplace_back(x(time + next_time), y(time + next_time), time + next_time);
        }

        line_spacing = !line_spacing;

        time += next_time;
        next_time = t(m_initial_velocity.x, m_initial_velocity.y - m_gravity * time);
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

void Trajectory::set_gravity(float gravity)
{
    m_gravity = gravity;
}

void Trajectory::set_color(const glm::vec4 &color)
{
    m_color = color;
}

float Trajectory::x(float time)
{
    return m_initial_position.x + m_initial_velocity.x * time * m_meter_as_pixels;
}

float Trajectory::y(float time)
{
    return m_initial_position.y + (m_initial_velocity.y * time - m_gravity * std::pow(time, 2) / 2.f) * m_meter_as_pixels;
}

float Trajectory::t(float vx, float vy)
{
    static constexpr uint32_t MAX_ITER = 100;

    float d = LINE_LENGTH / m_meter_as_pixels;
    float precision = d / 100.f;
    float theta = std::atan2(vy, vx);
    float dx = d * std::cos(theta);
    float time = dx / vx;

    float min_time = 0.f;
    float max_time = time * 10.f;

    // binary search till precision is reached
    for (int i = 0; i < MAX_ITER; ++i)
    {
        float xa = vx * time;
        float ya = vy * time - m_gravity * std::pow(time, 2.f) / 2.f;
        float da = std::hypot(xa, ya);

        float difference = d - da;

        if (std::fabs(difference) < precision)
            return time;

        if (difference > 0.f)
            min_time = time;
        else
            max_time = time;

        time = (min_time + max_time) / 2.f;
    }

    return time;
}

void Trajectory::set_meter_as_pixels(float meter_as_pixels)
{
    m_meter_as_pixels = meter_as_pixels;
}

void Trajectory::set_duration(float duration)
{
    m_duration = duration;
}
