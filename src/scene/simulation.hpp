//
// Created by Gianni on 17/06/2024.
//

#ifndef PROJECTILEMOTIONSIM_SIMULATION_HPP
#define PROJECTILEMOTIONSIM_SIMULATION_HPP

#include <imgui/imgui.h>
#include <memory>
#include "graph.hpp"
#include "vector.hpp"


class Simulation
{
public:
    Simulation(std::shared_ptr<Graph>& graph);

    void update(double dt);
    void render();

private:
    enum class State
    {
        INIT,
        RUNNING,
        PAUSED
    };

    enum SimulationSpeed
    {
        VERY_SLOW = 0,
        SLOW,
        NORMAL,
        FAST,
        VERY_FAST,
        COUNT
    };

private:
    void reset();
    void update_velocity();
    void update_position();
    void update_vectors();
    void render_vectors();
    void control_panel();
    void info_panel();

private:
    std::shared_ptr<Graph> m_graph;
    State m_state;

    float m_initial_velocity;
    int m_initial_angle;
    float m_initial_height;

    glm::vec2 m_velocity;
    glm::vec2 m_position;

    float m_gravity;
    SimulationSpeed m_simulation_speed;
    uint32_t m_duration;
    float m_vector_length;

    bool m_show_velocity_vector;
    bool m_show_velocity_vector_components;
    bool m_show_trajectory;

    Vector m_velocity_vector;
    Vector m_velocity_vector_x_component;
    Vector m_velocity_vector_y_component;
};


#endif //PROJECTILEMOTIONSIM_SIMULATION_HPP
