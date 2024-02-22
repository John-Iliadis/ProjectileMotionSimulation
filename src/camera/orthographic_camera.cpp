//
// Created by Gianni on 21/02/2024.
//

#include "orthographic_camera.hpp"


OrthographicCamera::OrthographicCamera(float width, float height)
    : m_projection_matrix(glm::ortho<float>(0, width, 0, height))
    , m_view_matrix(1)
    , m_view_projection_matrix(m_projection_matrix)
    , m_size(width, height)
    , m_position()
{
}

OrthographicCamera::OrthographicCamera()
    : m_projection_matrix(1)
    , m_view_matrix(1)
    , m_view_projection_matrix(1)
    , m_size()
    , m_position()
{
}

void OrthographicCamera::set_size(float width, float height)
{
    if (m_size == glm::vec2(width, height))
        return;

    m_size = {width, height};

    m_projection_matrix = glm::ortho<float>(0, width, 0, height);
    m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}

void OrthographicCamera::set_position(const glm::vec3 &position)
{
    m_position = position;
    recalculate_view_matrix();
}

void OrthographicCamera::set_position(float x, float y, float z)
{
    m_position = {x, y, z};
    recalculate_view_matrix();
}

const glm::vec2 &OrthographicCamera::get_size() const
{
    return m_size;
}

const glm::vec3 &OrthographicCamera::get_position() const
{
    return m_position;
}

const glm::mat4 &OrthographicCamera::get_projection_matrix() const
{
    return m_projection_matrix;
}

const glm::mat4 &OrthographicCamera::get_view_matrix() const
{
    return m_view_matrix;
}

const glm::mat4 &OrthographicCamera::get_view_projection_matrix() const
{
    return m_view_projection_matrix;
}

void OrthographicCamera::recalculate_view_matrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.f), m_position);

    m_view_matrix = glm::inverse(m_view_matrix);

    m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}
