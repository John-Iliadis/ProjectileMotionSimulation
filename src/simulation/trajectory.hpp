//
// Created by Gianni on 21/06/2024.
//

#ifndef PROJECTILEMOTIONSIM_TRAJECTORY_HPP
#define PROJECTILEMOTIONSIM_TRAJECTORY_HPP

#include <vector>
#include <cmath>
#include "../renderer2D/renderer2D.hpp"
#include "../utilities/utils.hpp"


struct LineSegment;

class Trajectory
{
public:
    Trajectory();
    Trajectory(const glm::vec2& initial_position,
               const glm::vec2& initial_velocity,
               const float gravity,
               const float meter_as_pixels,
               const glm::vec4& color = glm::vec4(1.f));

    void update(float simulation_time);
    void render();
    void make_lines();

    void set_initial_position(const glm::vec2& pos);
    void set_initial_velocity(const glm::vec2& vel);
    void set_gravity(float gravity);
    void set_meter_as_pixels(float meter_as_pixels);
    void set_duration(float duration);
    void set_color(const glm::vec4& color);

private:
    float x(float time);
    float y(float time);
    float t(float vx, float vy);

private:
    std::vector<LineSegment> m_lines;
    glm::vec2 m_initial_position;
    glm::vec2 m_initial_velocity;
    float m_gravity;
    float m_meter_as_pixels;
    float m_duration;
    float m_simulation_time;
    glm::vec4 m_color;
};


struct LineSegment
{
    float x;
    float y;
    float time;
};

#endif //PROJECTILEMOTIONSIM_TRAJECTORY_HPP
