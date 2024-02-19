//
// Created by Gianni on 19/02/2024.
//

#include "vertex_buffer_static.hpp"

VertexBufferStatic::VertexBufferStatic()
    : m_renderer_id()
    , m_count()
{
}

VertexBufferStatic::VertexBufferStatic(const void *data, uint32_t count)
    : m_count(count)
{
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(float), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBufferStatic::~VertexBufferStatic()
{
    delete_buffer();
}

VertexBufferStatic::VertexBufferStatic(VertexBufferStatic &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    m_count = other.m_count;

    other.delete_buffer();
}

VertexBufferStatic &VertexBufferStatic::operator=(VertexBufferStatic &&other) noexcept
{
    if (this != &other)
    {
        m_renderer_id = other.m_renderer_id;
        m_count = other.m_count;

        other.delete_buffer();
    }

    return *this;
}

void VertexBufferStatic::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void VertexBufferStatic::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint32_t VertexBufferStatic::get_count() const
{
    return m_count;
}

void VertexBufferStatic::delete_buffer()
{
    if (m_renderer_id)
    {
        glDeleteBuffers(1, &m_renderer_id);

        m_renderer_id = 0;
        m_count = 0;
    }
}
