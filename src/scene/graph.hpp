//
// Created by Gianni on 22/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_GRAPH_HPP
#define PROJECTILEMOTIONSIM_GRAPH_HPP

#include <array>
#include <vector>
#include <glad/glad.h>
#include "../renderer2D/renderer2D_includes.hpp"


struct Point
{
    glm::vec2 pos;
    Text text;
};

enum class ZoomCommand
{
    ZOOMING_IN,
    ZOOMING_OUT
};

// The graph of the projectile motion simulation
class Graph
{
public:
    Graph();

    void create(float width, float height);
    void render();
    void update(double dt);

    void zoom_in();
    void zoom_out();

    float get_meter_as_pixels() const;

private:
    void create_axes();
    void create_points();

private:
    std::array<glm::vec2, 4> m_axes;
    std::vector<Point> m_points;
    glm::vec2 m_graph_origin;
    FontAtlas m_arial;

    float m_width;
    float m_height;
    uint8_t m_zoom_level;
};


#endif //PROJECTILEMOTIONSIM_GRAPH_HPP
