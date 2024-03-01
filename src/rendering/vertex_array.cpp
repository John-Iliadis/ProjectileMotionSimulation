//
// Created by Gianni on 19/02/2024.
//

#include "vertex_array.hpp"

VertexArray::VertexArray()
    : m_renderer_id()
{
    glGenVertexArrays(1, &m_renderer_id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_renderer_id);
}

VertexArray::VertexArray(VertexArray &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept
{
    if (this != &other)
    {
        if (m_renderer_id)
        {
            glDeleteVertexArrays(1, &m_renderer_id);
        }

        m_renderer_id = other.m_renderer_id;
        other.m_renderer_id = 0;
    }

    return *this;
}

void VertexArray::bind() const
{
    glBindVertexArray(m_renderer_id);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::attach_vertex_buffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
    bind();
    vbo.bind();

    uint32_t offset{};

    for (const auto& attribute : layout.get_attributes())
    {
        glEnableVertexAttribArray(attribute.index);
        glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalize, layout.get_stride(), reinterpret_cast<void*>(offset));
        offset += attribute.size * utils::gl_sizeof(attribute.type);
    }

    unbind();
    vbo.unbind();
}

void VertexArray::attach_index_buffer(const IndexBufferStatic &ibo)
{
    bind();
    ibo.bind();

    unbind();
    ibo.unbind();
}
