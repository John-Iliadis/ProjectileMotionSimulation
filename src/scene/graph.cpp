//
// Created by Gianni on 22/02/2024.
//

#include "graph.hpp"


Graph::Graph()
    : m_width()
    , m_height()
    , m_graph_origin()
{
}

void Graph::create(float width, float height)
{
    m_width = width;
    m_height = height;

    m_graph_origin = glm::vec2(0.2 * m_height);

    m_points.clear();

    create_axes();
    create_points();
}

void Graph::update(double dt)
{
}

void Graph::render()
{
    // draw axes
    renderer2D::draw_line(m_axes[0], m_axes[1]);
    renderer2D::draw_line(m_axes[2], m_axes[3]);

    // draw points
    for (const auto& point : m_points)
        renderer2D::draw_point(point);
}

void Graph::create_axes()
{
    // x-axis
    m_axes[0] = {0, m_graph_origin.y};
    m_axes[1] = {m_width, m_graph_origin.y};

    // y-axis
    m_axes[2] = {m_graph_origin.x, 0};
    m_axes[3] = {m_graph_origin.x, m_height};
}

void Graph::create_points()
{
    float m_point_interval = m_height / 10;

    float pos_x = m_graph_origin.x;
    float neg_x = m_graph_origin.x - m_point_interval;
    float pos_y = m_graph_origin.y + m_point_interval;
    float neg_y = m_graph_origin.y - m_point_interval;

    // positive x-axis
    while (pos_x < m_width)
    {
        m_points.emplace_back(pos_x, m_graph_origin.y);
        pos_x += m_point_interval;
    }

    // negative x-axis
    while (neg_x > 0)
    {
        m_points.emplace_back(neg_x, m_graph_origin.y);
        neg_x -= m_point_interval;
    }

    // positive y-axis
    while (pos_y < m_height)
    {
        m_points.emplace_back(m_graph_origin.x, pos_y);
        pos_y += m_point_interval;
    }

    // negative y-axis
    while (neg_y > 0)
    {
        m_points.emplace_back(m_graph_origin.x, neg_y);
        neg_y -= m_point_interval;
    }
}
