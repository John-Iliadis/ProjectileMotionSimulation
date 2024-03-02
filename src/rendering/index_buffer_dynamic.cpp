//
// Created by Gianni on 2/03/2024.
//

#include "index_buffer_dynamic.hpp"


IndexBufferDynamic::IndexBufferDynamic()
    : m_renderer_id()
{
}

IndexBufferDynamic::IndexBufferDynamic(size_t size)
{
    glCreateBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBufferDynamic::~IndexBufferDynamic()
{
    glDeleteBuffers(1, &m_renderer_id);
}

IndexBufferDynamic::IndexBufferDynamic(IndexBufferDynamic &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
}

IndexBufferDynamic &IndexBufferDynamic::operator=(IndexBufferDynamic &&other) noexcept
{
    if (this != &other)
    {
        if (m_renderer_id)
        {
            glDeleteBuffers(1, &m_renderer_id);
        }

        m_renderer_id = other.m_renderer_id;
        other.m_renderer_id = 0;
    }

    return *this;
}

void IndexBufferDynamic::set_data(const void *data, uint32_t count)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBufferDynamic::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
}

void IndexBufferDynamic::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
