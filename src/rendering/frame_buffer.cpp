//
// Created by Gianni on 19/02/2024.
//

#include "frame_buffer.hpp"


FrameBuffer::FrameBuffer()
    : m_renderer_id()
    , m_texture_id()
    , m_width()
    , m_height()
{
}

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
    : m_renderer_id()
    , m_texture_id()
    , m_width(width)
    , m_height(height)
{
    create();
}

FrameBuffer::~FrameBuffer()
{
    destroy();
}

FrameBuffer::FrameBuffer(FrameBuffer &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    m_texture_id = other.m_texture_id;
    m_width = other.m_width;
    m_height = other.m_height;

    other.destroy();
}

FrameBuffer &FrameBuffer::operator=(FrameBuffer &&other) noexcept
{
    if (this != &other)
    {
        m_renderer_id = other.m_renderer_id;
        m_texture_id = other.m_texture_id;
        m_width = other.m_width;
        m_height = other.m_height;

        other.destroy();
    }

    return *this;
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
    glViewport(0, 0, m_width, m_height);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0)
    {
        std::cout << "FrameBuffer::resize: Attempted to resize framebuffer to " +
                     std::to_string(width) + 'x' + std::to_string(height) + '\n';

        return;
    }

    if (m_width == width && m_height == height)
        return;

    m_width = width;
    m_height = height;

    create();
}

void FrameBuffer::destroy()
{
    if (m_renderer_id)
    {
        glDeleteTextures(1, &m_texture_id); // todo: used multi-sampled texture
        glDeleteFramebuffers(1, &m_renderer_id);

        m_renderer_id = 0;
        m_texture_id = 0;
        m_width = 0;
        m_height = 0;
    }
}

uint32_t FrameBuffer::get_texture_id() const
{
    return m_texture_id;
}

uint32_t FrameBuffer::get_width() const
{
    return m_width;
}

uint32_t FrameBuffer::get_height() const
{
    return m_height;
}

void FrameBuffer::create()
{
    destroy();

    glGenFramebuffers(GL_FRAMEBUFFER, &m_renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    // todo: used multi-sampled texture
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_id, 0);

    uint32_t frame_buffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (frame_buffer_status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("FrameBuffer::create: Failed to create the framebuffer\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
