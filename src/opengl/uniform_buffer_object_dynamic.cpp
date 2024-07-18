//
// Created by Gianni on 18/06/2024.
//

#include "uniform_buffer_object_dynamic.hpp"


UniformBufferObjectDynamic::UniformBufferObjectDynamic()
    : m_renderer_id()
    , m_index()
{
}

UniformBufferObjectDynamic::UniformBufferObjectDynamic(const uint32_t buffer_size, const uint32_t index)
    : m_renderer_id()
    , m_index(index)
{
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, m_renderer_id);
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_index, m_renderer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBufferObjectDynamic::~UniformBufferObjectDynamic()
{
    glDeleteBuffers(1, &m_renderer_id);
}

UniformBufferObjectDynamic::UniformBufferObjectDynamic(UniformBufferObjectDynamic &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    m_index = other.m_index;
    other.m_renderer_id = 0;
    other.m_index = 0;
}

UniformBufferObjectDynamic &UniformBufferObjectDynamic::operator=(UniformBufferObjectDynamic &&other) noexcept
{
    if (this != &other)
    {
        if (m_renderer_id)
        {
            glDeleteBuffers(1, &m_renderer_id);
        }

        m_renderer_id = other.m_renderer_id;
        m_index = other.m_index;
        other.m_renderer_id = 0;
        other.m_index = 0;
    }

    return *this;
}

void UniformBufferObjectDynamic::set_data(const void *data, const uint32_t buffer_sze)
{
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer_sze, data);
    unbind();
}

void UniformBufferObjectDynamic::bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_renderer_id);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_index, m_renderer_id);
}

void UniformBufferObjectDynamic::unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

uint32_t UniformBufferObjectDynamic::get_index() const
{
    return 0;
}
