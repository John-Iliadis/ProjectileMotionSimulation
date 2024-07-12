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
    int value;
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

    void zoom_in(int count = 1);
    void zoom_out(int count = 1);

    float get_meter_as_pixels() const;
    uint8_t get_zoom_level() const;
    uint8_t get_max_zoom_level() const;
    const glm::vec2& get_origin() const;

private:
    void create_axes();
    void create_points();
    void create_pos_x_axis_points();
    void create_neg_x_axis_points();
    void create_pos_y_axis_points();
    void create_neg_y_axis_points();
    Point create_point(float x, float y, int value);

private:
    std::array<glm::vec2, 4> m_axes;
    std::vector<Point> m_points;
    FontAtlas m_arial;

    float m_width;
    float m_height;

    glm::vec2 m_graph_origin;
    float m_point_interval;
    uint32_t m_value_interval;

    uint8_t m_zoom_level;
};


#endif //PROJECTILEMOTIONSIM_GRAPH_HPP
