//
// Created by Gianni on 22/02/2024.
//

#include "graph.hpp"


constexpr std::array<float, 10> scale {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};

Graph::Graph()
    : m_width()
    , m_height()
    , m_zoom_level()
    , m_graph_origin()
    , m_arial("../assets/fonts/arial.ttf", 15)
{
}

void Graph::create(float width, float height)
{
    m_width = width;
    m_height = height;

    m_graph_origin = glm::vec2(0.2 * m_height);

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
    {
        renderer2D::draw_point(point.pos);
        renderer2D::draw_text(point.text);
    }
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
    m_points.clear();

    static constexpr int number_of_vertical_points = 10;

    const float point_interval = m_height / number_of_vertical_points;
    const uint32_t value = scale.at(m_zoom_level) / number_of_vertical_points;

    int point_value;

    // positive x-axis
    float pos_x = m_graph_origin.x + point_interval;
    point_value = value;
    while (pos_x < m_width)
    {
        Point point;
        point.pos = glm::vec2(pos_x, m_graph_origin.y);
        point.text = Text(&m_arial, std::to_string(point_value), {pos_x, m_graph_origin.y - 10}, TextOrigin::TOP_CENTER);

        m_points.push_back(std::move(point));
        pos_x += point_interval;
        point_value += value;
    }

    // negative x-axis
    float neg_x = m_graph_origin.x - point_interval;
    point_value = -value;
    while (neg_x > 0)
    {
        Point point;
        point.pos = glm::vec2(neg_x, m_graph_origin.y);
        point.text = Text(&m_arial, std::to_string(point_value), {neg_x, m_graph_origin.y - 10}, TextOrigin::TOP_CENTER);

        m_points.push_back(std::move(point));
        neg_x -= point_interval;
        point_value -= value;
    }

    // positive y-axis
    float pos_y = m_graph_origin.y + point_interval;
    point_value = value;
    while (pos_y < m_height)
    {
        Point point;
        point.pos = glm::vec2(m_graph_origin.x, pos_y);
        point.text = Text(&m_arial, std::to_string(point_value), {m_graph_origin.x - 10, pos_y}, TextOrigin::CENTER_RIGHT);

        m_points.push_back(std::move(point));
        pos_y += point_interval;
        point_value += value;
    }

    // negative y-axis
    float neg_y = m_graph_origin.y - point_interval;
    point_value = -value;
    while (neg_y > 0)
    {
        Point point;
        point.pos = glm::vec2(m_graph_origin.x, neg_y);
        point.text = Text(&m_arial, std::to_string(point_value), {m_graph_origin.x - 10, neg_y}, TextOrigin::CENTER_RIGHT);

        m_points.push_back(std::move(point));
        neg_y -= point_interval;
        point_value -= value;
    }
}

float Graph::get_meter_as_pixels() const
{
    return m_height / scale.at(m_zoom_level);
}

void Graph::zoom_in()
{

}

void Graph::zoom_out()
{

}
