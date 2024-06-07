//
// Created by Gianni on 2/03/2024.
//

#include "vertex_buffer_dynamic.hpp"


VertexBufferDynamic::VertexBufferDynamic()
    : m_renderer_id()
{
}

VertexBufferDynamic::VertexBufferDynamic(size_t buffer_size)
{
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBufferDynamic::~VertexBufferDynamic()
{
    glDeleteBuffers(1, &m_renderer_id);
}

VertexBufferDynamic::VertexBufferDynamic(VertexBufferDynamic &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
}

VertexBufferDynamic &VertexBufferDynamic::operator=(VertexBufferDynamic &&other) noexcept
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

void VertexBufferDynamic::set_data(const void *data, uint32_t size_of_memory_block)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size_of_memory_block, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferDynamic::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void VertexBufferDynamic::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
