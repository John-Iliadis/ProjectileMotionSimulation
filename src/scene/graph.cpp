//
// Created by Gianni on 22/02/2024.
//

#include "graph.hpp"


static constexpr std::array<float, 10> scale {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};
static constexpr uint32_t VERTICAL_SEGMENTS = 10;

Graph::Graph()
    : m_width()
    , m_height()
    , m_graph_origin()
    , m_point_interval()
    , m_value_interval()
    , m_zoom_level()
    , m_arial("../assets/fonts/arial.ttf", 15)
{
}

void Graph::create(float width, float height)
{
    m_width = width;
    m_height = height;

    m_graph_origin = glm::vec2(0.2 * m_height);
    m_point_interval = m_height / static_cast<float>(VERTICAL_SEGMENTS); // number of pixels between two points
    m_value_interval = scale.at(m_zoom_level) / VERTICAL_SEGMENTS; // the change

    create_axes();
    create_points();
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

    m_points.push_back(create_point(m_graph_origin.x, m_graph_origin.y, 0)); // graph origin
    create_pos_x_axis_points();
    create_neg_x_axis_points();
    create_pos_y_axis_points();
    create_neg_y_axis_points();
}

float Graph::get_meter_as_pixels() const
{
    return m_height / scale.at(m_zoom_level);
}

void Graph::zoom_in()
{
    if (m_zoom_level - 1 >= 0)
    {
        --m_zoom_level;

        const float change = static_cast<float>(scale.at(m_zoom_level)) / static_cast<float>(scale.at(m_zoom_level + 1));

        for (Point& point : m_points)
        {
            point.value *= change;
            point.text.string = std::to_string(point.value);
        }
    }
}

void Graph::zoom_out()
{
    if (m_zoom_level + 1 < scale.size())
    {
        ++m_zoom_level;

        const float change = static_cast<float>(scale.at(m_zoom_level)) / static_cast<float>(scale.at(m_zoom_level - 1));

        for (Point& point : m_points)
        {
            point.value *= change;
            point.text.string = std::to_string(point.value);
        }
    }
}

uint8_t Graph::get_zoom_level() const
{
    return m_zoom_level;
}

uint8_t Graph::get_max_zoom_level() const
{
    return scale.size() - 1;
}

void Graph::create_pos_x_axis_points()
{
    float x_pos = m_graph_origin.x + m_point_interval;
    float y_pos = m_graph_origin.y;
    int point_value = m_value_interval;

    while (x_pos < m_width)
    {
        m_points.push_back(create_point(x_pos, y_pos, point_value));
        x_pos += m_point_interval;
        point_value += m_value_interval;
    }
}

void Graph::create_neg_x_axis_points()
{
    float x_pos = m_graph_origin.x - m_point_interval;
    float y_pos = m_graph_origin.y;
    int point_value = -m_value_interval;

    while (x_pos > 0)
    {
        m_points.push_back(create_point(x_pos, y_pos, point_value));
        x_pos -= m_point_interval;
        point_value -= m_value_interval;
    }
}

void Graph::create_pos_y_axis_points()
{
    float x_pos = m_graph_origin.x;
    float y_pos = m_graph_origin.y + m_point_interval;
    int point_value = m_value_interval;

    while (y_pos < m_height)
    {
        m_points.push_back(create_point(x_pos, y_pos, point_value));
        y_pos += m_point_interval;
        point_value += m_value_interval;
    }
}

void Graph::create_neg_y_axis_points()
{
    float x_pos = m_graph_origin.x;
    float y_pos = m_graph_origin.y - m_point_interval;
    int point_value = -m_value_interval;

    while (y_pos > 0)
    {
        m_points.push_back(create_point(x_pos, y_pos, point_value));
        y_pos -= m_point_interval;
        point_value -= m_value_interval;
    }
}

Point Graph::create_point(float x, float y, int value)
{
    Point point;

    point.pos = glm::vec2(x, y);
    point.value = value;

    point.text.font_atlas = &m_arial;
    point.text.string = std::to_string(value);
    point.text.position = glm::vec2(x, y);

    constexpr float TEXT_MARGIN = 10.f;

    if (x == m_graph_origin.x && y == m_graph_origin.y)
    {
        point.text.origin = TextOrigin::TOP_RIGHT;
        point.text.position.x -= TEXT_MARGIN;
        point.text.position.y -= TEXT_MARGIN;
    }
    else if (y == m_graph_origin.y)
    {
        point.text.origin = TextOrigin::TOP_CENTER;
        point.text.position.y -= TEXT_MARGIN;
    }
    else
    {
        point.text.origin = TextOrigin::CENTER_RIGHT;
        point.text.position.x -= TEXT_MARGIN;
    }

    return point;
}
