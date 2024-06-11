//
// Created by Gianni on 22/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_GRAPH_HPP
#define PROJECTILEMOTIONSIM_GRAPH_HPP

#include <array>
#include <vector>
#include <glad/glad.h>
#include "../renderer2D/renderer2D_includes.hpp"


// The graph of the projectile motion simulation
class Graph
{
public:
    Graph();

    void create(float width, float height);
    void render();
    void update(double dt);

private:
    void create_axes();
    void create_points();

private:
    std::array<glm::vec2, 4> m_axes;
    std::vector<glm::vec2> m_points;
    glm::vec2 m_graph_origin;

    float m_width;
    float m_height;
};


#endif //PROJECTILEMOTIONSIM_GRAPH_HPP
