//
// Created by Gianni on 19/02/2024.
//

#include "frame_buffer.hpp"


FrameBuffer::FrameBuffer()
    : m_texture_type()
    , m_renderer_id()
    , m_texture_id()
    , m_width()
    , m_height()
{
}

FrameBuffer::FrameBuffer(uint32_t texture_type, uint32_t width, uint32_t height)
    : m_texture_type(texture_type)
    , m_renderer_id()
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
    m_texture_type = other.m_texture_type;
    m_renderer_id = other.m_renderer_id;
    m_texture_id = other.m_texture_id;
    m_width = other.m_width;
    m_height = other.m_height;

    other.m_renderer_id = 0;
    other.m_texture_id = 0;
    other.m_width = 0;
    other.m_height = 0;
}

FrameBuffer &FrameBuffer::operator=(FrameBuffer &&other) noexcept
{
    if (this != &other)
    {
        if (m_renderer_id)
        {
            destroy();
        }

        m_texture_type = other.m_texture_type;
        m_renderer_id = other.m_renderer_id;
        m_texture_id = other.m_texture_id;
        m_width = other.m_width;
        m_height = other.m_height;

        other.m_renderer_id = 0;
        other.m_texture_id = 0;
        other.m_width = 0;
        other.m_height = 0;
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
                     std::to_string(width) + 'x' + std::to_string(height) << std::endl;

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
        glDeleteTextures(1, &m_texture_id);
        glDeleteFramebuffers(1, &m_renderer_id);

        m_renderer_id = 0;
        m_texture_id = 0;
    }
}

uint32_t FrameBuffer::get_framebuffer_id() const
{
    return m_renderer_id;
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

    switch (m_texture_type)
    {
        case GL_TEXTURE_2D:
            gen_framebuffer(m_renderer_id, m_texture_id, m_width, m_height);
            break;

        case GL_TEXTURE_2D_MULTISAMPLE:
            gen_framebuffer_msaa(m_renderer_id, m_texture_id, m_width, m_height);
            break;

        default:
            throw std::runtime_error("FrameBuffer::create: Invalid texture type\n");
    }
}

void FrameBuffer::gen_framebuffer(uint32_t &renderer_id, uint32_t &texture_id, uint32_t width, uint32_t height)
{
    glGenFramebuffers(1, &renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer_id);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

    uint32_t frame_buffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (frame_buffer_status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("gen_framebuffer: Failed to create the framebuffer\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::gen_framebuffer_msaa(uint32_t &renderer_id, uint32_t &texture_id, uint32_t width, uint32_t height)
{
    glGenFramebuffers(1, &renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer_id);

    uint32_t samples = 4;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture_id, 0);

    uint32_t frame_buffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (frame_buffer_status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("gen_framebuffer_msaa: Failed to create the framebuffer\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
