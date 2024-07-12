//
// Created by Gianni on 16/06/2024.
//

#include "vector.hpp"


static std::once_flag flag;
static std::unique_ptr<Shader> vector_shader;
static std::unique_ptr<IndexBufferStatic> vector_ibo;
static VertexBufferLayout vector_buffer_layout;

static constexpr uint32_t VECTOR_VERTEX_COUNT = 7;
static constexpr uint32_t VECTOR_VERTEX_SIZE = 2 * sizeof(float);
static constexpr uint32_t VECTOR_INDEX_COUNT = 9;
static constexpr uint32_t vector_indices[VECTOR_INDEX_COUNT]
{
    0, 1, 2,
    0, 2, 3,
    4, 5, 6
};


Vector::Vector()
    : Vector(glm::vec2(), glm::vec2(), 0)
{
}

Vector::Vector(const glm::vec2 &pos, float magnitude, float angle, float meter_as_pixels, const glm::vec4 &color)
    : Vector(pos,
             {magnitude * glm::cos(glm::radians(angle)), magnitude * glm::sin(glm::radians(angle))},
             meter_as_pixels,
             color)
{
}

Vector::Vector(const glm::vec2 &pos, const glm::vec2 &magnitude, float meter_as_pixels, const glm::vec4 &color)
    : m_position(pos)
    , m_magnitude(magnitude)
    , m_color(color)
    , m_meter_as_pixels(meter_as_pixels)
    , m_magnification(1.f)
{
    std::call_once(flag, Vector::init);

    m_vbo = VertexBufferDynamic(VECTOR_VERTEX_COUNT * VECTOR_VERTEX_SIZE);

    m_vao.attach_vertex_buffer(m_vbo, vector_buffer_layout);
    m_vao.attach_index_buffer(*vector_ibo);
}

void Vector::render()
{
    render_impl(*this);
}

void Vector::set_position(const glm::vec2 &position)
{
    m_position = position;
}

void Vector::set_position(float x, float y)
{
    m_position = {x, y};
}

void Vector::set_magnitude(const glm::vec2 &magnitude)
{
    m_magnitude = magnitude;
}

void Vector::set_magnitude(float x, float y)
{
    m_magnitude = {x, y};
}

void Vector::set_color_rgb(float r, float g, float b)
{
    m_color = {r / 255.f, g / 255.f, b / 255.f, 1.f};
}

void Vector::set_meter_as_pixels(float meter_as_pixels)
{
    m_meter_as_pixels = meter_as_pixels;
}

void Vector::set_magnification(float magnification)
{
    m_magnification = magnification;
}

void Vector::init()
{
    vector_shader = std::make_unique<Shader>("../shaders/vector.vert", "../shaders/vector.frag");
    vector_ibo = std::make_unique<IndexBufferStatic>(vector_indices, 9);
    vector_buffer_layout = {{0, 2, GL_FLOAT, GL_TRUE}};
}

void Vector::render_impl(const Vector &vector)
{
    constexpr float precision = 1e-2f;
    if (std::fabs(vector.m_magnitude.x) < precision && std::fabs(vector.m_magnitude.y) < precision)
        return;

    const glm::vec2& pos = vector.m_position;
    const glm::vec2& magnitude = vector.m_magnitude;

    const float vector_length = std::hypot(magnitude.x, magnitude.y) * vector.m_meter_as_pixels * vector.m_magnification;
    const float arrow_head_length = glm::clamp(vector_length, 0.f, 20.f);
    const float arrow_head_width = arrow_head_length / 2.f;
    const float shaft_width = 5.f;
    const float shaft_length = vector_length > arrow_head_length? vector_length - arrow_head_length : 0.f;

    const float vertices[]
    {
        pos.x, pos.y - shaft_width / 2.f,
        pos.x, pos.y + shaft_width / 2.f,
        pos.x + shaft_length, pos.y + shaft_width / 2.f,
        pos.x + shaft_length, pos.y - shaft_width / 2.f,

        pos.x + shaft_length, pos.y - arrow_head_width / 2.f,
        pos.x + shaft_length, pos.y + arrow_head_width / 2.f,
        pos.x + shaft_length + arrow_head_length, pos.y
    };

    const float angle = std::atan2(magnitude.y, magnitude.x);
    glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(pos, 0));
    model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
    model = glm::translate(model, glm::vec3(-pos, 0));

    vector.m_vbo.set_data(vertices, VECTOR_VERTEX_COUNT * VECTOR_VERTEX_SIZE);

    // render here
    vector.m_vao.bind();
    vector_shader->bind();
    vector_shader->set_mat4("u_model", model);
    vector_shader->set_float4("u_color", vector.m_color);

    glDrawElements(GL_TRIANGLES, VECTOR_INDEX_COUNT, GL_UNSIGNED_INT, nullptr);

    vector.m_vao.unbind();
    vector_shader->unbind();
}
