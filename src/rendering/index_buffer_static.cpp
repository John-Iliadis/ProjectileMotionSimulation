//
// Created by Gianni on 19/02/2024.
//

#include "index_buffer_static.hpp"


IndexBufferStatic::IndexBufferStatic()
    : m_renderer_id()
    , m_count()
{
}

IndexBufferStatic::IndexBufferStatic(const void *data, uint32_t count)
    : m_count(count)
{
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBufferStatic::~IndexBufferStatic()
{
    delete_buffer();
}

IndexBufferStatic::IndexBufferStatic(IndexBufferStatic &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    m_count = other.m_count;

    other.delete_buffer();
}

IndexBufferStatic &IndexBufferStatic::operator=(IndexBufferStatic &&other) noexcept
{
    if (this != &other)
    {
        m_renderer_id = other.m_renderer_id;
        m_count = other.m_count;

        other.delete_buffer();
    }

    return *this;
}

void IndexBufferStatic::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
}

void IndexBufferStatic::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t IndexBufferStatic::get_count() const
{
    return m_count;
}

void IndexBufferStatic::delete_buffer()
{
    if (m_renderer_id)
    {
        glDeleteBuffers(1, &m_renderer_id);

        m_renderer_id = 0;
        m_count = 0;
    }
}
