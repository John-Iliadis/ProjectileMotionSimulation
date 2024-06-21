//
// Created by Gianni on 21/06/2024.
//

#ifndef PROJECTILEMOTIONSIM_TRAJECTORY_HPP
#define PROJECTILEMOTIONSIM_TRAJECTORY_HPP

#include <vector>
#include "../renderer2D/renderer2D.hpp"
#include "../utilities/utils.hpp"


class Trajectory
{
public:
    Trajectory();
    Trajectory(const glm::vec2& initial_position,
               const glm::vec2& initial_velocity,
               const float gravity,
               const float meter_as_pixels,
               const glm::vec4& color = glm::vec4(1.f));

    void update(const float simulation_time);
    void render();

    void set_initial_position(const glm::vec2& pos);
    void set_initial_velocity(const glm::vec2& vel);
    void set_gravity(const float gravity);
    void set_meter_as_pixels(const float meter_as_pixels);
    void set_color(const glm::vec4& color);
    void clear();

private:
    float x(const float time);
    float y(const float time);

private:
    std::vector<glm::vec2> m_lines;
    glm::vec2 m_initial_position;
    glm::vec2 m_initial_velocity;
    float m_gravity;
    float m_meter_as_pixels;
    glm::vec4 m_color;
};


#endif //PROJECTILEMOTIONSIM_TRAJECTORY_HPP
