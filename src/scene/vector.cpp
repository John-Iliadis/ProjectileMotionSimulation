//
// Created by Gianni on 16/06/2024.
//

#include "vector.hpp"


Vector::Vector()
    : Vector(glm::vec2(), glm::vec2(), 0)
{
}

Vector::Vector(const glm::vec2 &pos, float velocity, float angle, float meter_as_pixels, const glm::vec4 &color)
        : Vector(pos,
                 {velocity * glm::cos(glm::radians(angle)), velocity * glm::sin(glm::radians(angle))},
                 meter_as_pixels,
                 color)
{
}

Vector::Vector(const glm::vec2 &pos, const glm::vec2 &vel, float meter_as_pixels, const glm::vec4 &color)
    : m_position(pos)
    , m_velocity(vel)
    , m_meter_as_pixels(meter_as_pixels)
    , m_color(color)
    , m_shader("../shaders/vector.vert", "../shaders/vector.frag")
    , m_view_proj()
    , m_model(1)
{
    m_vbo = VertexBufferDynamic(7 * 2 * sizeof(float));

    constexpr uint32_t indices[]
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6
    };

    m_ibo = IndexBufferStatic(indices, 9);

    VertexBufferLayout layout{{0, 2, GL_FLOAT, GL_FALSE}};

    m_vao.attach_vertex_buffer(m_vbo, layout);
    m_vao.attach_index_buffer(m_ibo);
}

void Vector::render()
{
    constexpr float arrow_head_length = 20;
    constexpr float shaft_width = 5;
    constexpr float arrow_head_width = 10;
    const float vector_length = std::sqrt(std::pow(m_velocity.x, 2) + std::pow(m_velocity.y, 2));
    const float angle = std::atan(m_velocity.y / m_velocity.x);
    const float vector_length_scaled = vector_length * m_meter_as_pixels;
    const float shaft_length = glm::clamp(vector_length_scaled - arrow_head_length, 0.f, std::numeric_limits<float>::max()) ;

    const float vertices[]
    {
        m_position.x, m_position.y - shaft_width / 2.f,
        m_position.x, m_position.y + shaft_width / 2.f,
        m_position.x + shaft_length, m_position.y + shaft_width / 2.f,
        m_position.x + shaft_length, m_position.y - shaft_width / 2.f,

        m_position.x + shaft_length, m_position.y - arrow_head_width / 2.f,
        m_position.x + shaft_length, m_position.y + arrow_head_width / 2.f,
        m_position.x + shaft_length + arrow_head_length, m_position.y
    };

    m_model = glm::translate(glm::mat4(1), glm::vec3(m_position, 0));
    m_model = glm::rotate(m_model, angle, glm::vec3(0, 0, 1));
    m_model = glm::translate(m_model, glm::vec3(-m_position, 0));

    m_vbo.set_data(vertices, 7 * 2 * sizeof(float));

    // render here
    m_vao.bind();
    m_shader.bind();
    m_shader.set_mat4("u_mvp_matrix", *m_view_proj * m_model);
    m_shader.set_float4("u_color", m_color);

    glDrawElements(GL_TRIANGLES, m_ibo.get_count(), GL_UNSIGNED_INT, nullptr);

    m_vao.unbind();
    m_shader.unbind();
}

void Vector::set_position(const glm::vec2 &position)
{
    m_position = position;
}

void Vector::set_position(float x, float y)
{
    m_position = {x, y};
}

void Vector::set_velocity(const glm::vec2 &velocity)
{
    m_velocity = velocity;
}

void Vector::set_velocity(float x, float y)
{
    m_velocity = {x, y};
}

void Vector::set_meter_as_pixels(float meter_as_pixels)
{
    m_meter_as_pixels = meter_as_pixels;
}

void Vector::set_view_proj_matrix(const glm::mat4 &matrix)
{
    m_view_proj = &matrix;
}
