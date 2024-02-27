//
// Created by Gianni on 22/02/2024.
//

#include "graph.hpp"


Graph::Graph()
    : m_width()
    , m_height()
{
}

void Graph::create(float width, float height)
{
    m_width = width;
    m_height = height;

    float origin_offset = 0.2 * height;

    m_vertices.clear();

    create_axes(width, height, origin_offset);
    create_points(width, height, origin_offset);

    m_vbo = VertexBufferStatic(m_vertices.data(), m_vertices.size());

    VertexBufferLayout layout {{0, 2, GL_FLOAT, GL_FALSE}};

    m_vao.attach_vertex_buffer(m_vbo, layout);

    glPointSize(5);
}

void Graph::render()
{
    m_vao.bind();

    assert(!m_vertices.empty());

    glDrawArrays(GL_LINES, 0, 4);
    glDrawArrays(GL_POINTS, 4, m_vertices.size() / 2 - 4);

    m_vao.unbind();
}

void Graph::create_axes(float width, float height, float origin_offset)
{
    std::array<float, 4> x_axis {
            0, origin_offset,
            width, origin_offset
    };

    std::array<float, 4> y_axis {
            origin_offset, 0,
            origin_offset, height
    };

    m_vertices.insert(m_vertices.begin(), x_axis.begin(), x_axis.end());
    m_vertices.insert(m_vertices.begin(), y_axis.begin(), y_axis.end());
}

void Graph::create_points(float width, float height, float origin_offset)
{
    float m_point_interval = height / 10;

    float pos_x = origin_offset;
    float neg_x = origin_offset - m_point_interval;
    float pos_y = origin_offset + m_point_interval;
    float neg_y = origin_offset - m_point_interval;

    while (pos_x < width)
    {
        float x = pos_x;
        float y = origin_offset;

        m_vertices.push_back(x);
        m_vertices.push_back(y);

        pos_x += m_point_interval;
    }

    while (neg_x > 0)
    {
        float x = neg_x;
        float y = origin_offset;

        m_vertices.push_back(x);
        m_vertices.push_back(y);

        neg_x -= m_point_interval;
    }

    while (pos_y < height)
    {
        float x = origin_offset;
        float y = pos_y;

        m_vertices.push_back(x);
        m_vertices.push_back(y);

        pos_y += m_point_interval;
    }

    while (neg_y > 0)
    {
        float x = origin_offset;
        float y = neg_y;

        m_vertices.push_back(x);
        m_vertices.push_back(y);

        neg_y -= m_point_interval;
    }
}
