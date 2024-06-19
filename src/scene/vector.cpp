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

void Vector::init()
{
    vector_shader = std::make_unique<Shader>("../shaders/vector.vert", "../shaders/vector.frag");
    vector_ibo = std::make_unique<IndexBufferStatic>(vector_indices, 9);
    vector_buffer_layout = {{0, 2, GL_FLOAT, GL_TRUE}};
}

void Vector::render_impl(const Vector &vector)
{
    constexpr float e = 1e-2f;
    if (std::abs(vector.m_velocity.x) < e && std::abs(vector.m_velocity.y) < e)
        return;

    const glm::vec2& pos = vector.m_position;
    const glm::vec2& velocity = vector.m_velocity;

    constexpr float ARROW_HEAD_LENGTH = 20.f;
    constexpr float ARROW_HEAD_WIDTH = 10.f;
    constexpr float SHAFT_WIDTH = 5.f;

    const float vector_length = std::hypot(velocity.x, velocity.y) * vector.m_meter_as_pixels;
    const float shaft_length = glm::clamp(vector_length - ARROW_HEAD_LENGTH, 0.f, std::numeric_limits<float>::max());

    const float vertices[]
    {
        pos.x, pos.y - SHAFT_WIDTH / 2.f,
        pos.x, pos.y + SHAFT_WIDTH / 2.f,
        pos.x + shaft_length, pos.y + SHAFT_WIDTH / 2.f,
        pos.x + shaft_length, pos.y - SHAFT_WIDTH / 2.f,

        pos.x + shaft_length, pos.y - ARROW_HEAD_WIDTH / 2.f,
        pos.x + shaft_length, pos.y + ARROW_HEAD_WIDTH / 2.f,
        pos.x + shaft_length + ARROW_HEAD_LENGTH, pos.y
    };

    const float angle = std::atan2(velocity.y, velocity.x);
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
