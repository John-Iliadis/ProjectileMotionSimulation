//
// Created by Gianni on 19/02/2024.
//

#include "vertex_array.hpp"

VertexArray::VertexArray()
    : m_renderer_id()
    , m_vertex_count()
    , m_index_count()
{
    glGenVertexArrays(1, &m_renderer_id);
}

VertexArray::~VertexArray()
{
    if (m_renderer_id)
    {
        glDeleteVertexArrays(1, &m_renderer_id);

        m_renderer_id = 0;
        m_vertex_count = 0;
        m_index_count = 0;
    }
}

VertexArray::VertexArray(VertexArray &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    m_vertex_count = other.m_vertex_count;
    m_index_count = other.m_index_count;

    other.m_renderer_id = 0;
    other.m_vertex_count = 0;
    other.m_index_count = 0;
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept
{
    if (this != &other)
    {
        m_renderer_id = other.m_renderer_id;
        m_vertex_count = other.m_vertex_count;
        m_index_count = other.m_index_count;

        other.m_renderer_id = 0;
        other.m_vertex_count = 0;
        other.m_index_count = 0;
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

void VertexArray::attach_vertex_buffer(const VertexBufferStatic &vbo, const VertexBufferLayout &layout)
{
    m_vertex_count = vbo.get_count();

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
    m_index_count = ibo.get_count();

    bind();
    ibo.bind();

    unbind();
    ibo.unbind();
}

uint32_t VertexArray::get_vertex_count() const
{
    return m_vertex_count;
}

uint32_t VertexArray::get_index_count() const
{
    return m_index_count;
}
