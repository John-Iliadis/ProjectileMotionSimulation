//
// Created by Gianni on 20/06/2024.
//

#include "projectile.hpp"

static std::once_flag flag;
static std::unique_ptr<Shader> shader;
static std::unique_ptr<IndexBufferStatic> ibo;
static constexpr uint32_t buffer_size = 4 * 4 * sizeof(float);

static void init()
{
    shader = std::make_unique<Shader>("../shaders/textured_quad.vert", "../shaders/textured_quad.frag");

    constexpr uint32_t indices[6] {0, 1, 2, 0, 2, 3 };
    ibo = std::make_unique<IndexBufferStatic>(indices, 6);
}

Projectile::Projectile(const glm::vec2 &size, const glm::vec2 &position)
    : m_texture("../assets/textures/projectile.png")
    , m_size(size)
    , m_position(position)
{
    std::call_once(flag, init);

    m_vbo = VertexBufferDynamic(buffer_size);

    VertexBufferLayout layout
    {
        {0, 2, GL_FLOAT, GL_FALSE},
        {1, 2, GL_FLOAT, GL_FALSE}
    };

    m_vao.attach_vertex_buffer(m_vbo, layout);
    m_vao.attach_index_buffer(*ibo);
}

void Projectile::render()
{
    const glm::vec2 pos = m_position - (m_size / 2.f);

    const float vertices[]
    {
        pos.x, pos.y, 0.f, 0.f,
        pos.x, pos.y + m_size.y, 0.f, 1.f,
        pos.x + m_size.x, pos.y + m_size.y, 1.f, 1.f,
        pos.x + m_size.x, pos.y, 1.f, 0.f
    };

    m_vbo.set_data(vertices, buffer_size);

    m_texture.bind();
    m_vao.bind();
    shader->bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    m_texture.unbind();
    m_vao.unbind();
    shader->unbind();
}

void Projectile::set_size(float width, float height)
{
    m_size = {width, height};
}

void Projectile::set_size(const glm::vec2 &size)
{
    m_size = size;
}

void Projectile::set_position(float x, float y)
{
    m_position = {x, y};
}

void Projectile::set_position(const glm::vec2 &position)
{
    m_position = position;
}
