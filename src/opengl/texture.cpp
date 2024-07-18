//
// Created by Gianni on 20/06/2024.
//

#include "texture.hpp"


Texture::Texture()
    : m_renderer_id()
    , m_width()
    , m_height()
{
}

Texture::Texture(const char *file_name)
    : Texture()
{
    stbi_set_flip_vertically_on_load(true);

    int channels;
    uint8_t* data = stbi_load(file_name, &m_width, &m_height, &channels, 0);

    if (!data)
    {
        throw std::runtime_error("Texture::Texture: Failed to load" + std::string(file_name));
    }

    int format;

    switch (channels)
    {
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: assert(false);
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_renderer_id);
}

Texture::Texture(Texture &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    m_width = other.m_width;
    m_height = other.m_height;
    other.m_renderer_id = 0;
    other.m_width = 0;
    other.m_height = 0;
}

Texture &Texture::operator=(Texture &&other) noexcept
{
    if (this != &other)
    {
        if (m_renderer_id)
        {
            glDeleteTextures(1, &m_renderer_id);
        }

        m_renderer_id = other.m_renderer_id;
        m_width = other.m_width;
        m_height = other.m_height;
        other.m_renderer_id = 0;
        other.m_width = 0;
        other.m_height = 0;
    }

    return *this;
}

void Texture::bind(uint32_t texture_unit)
{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t Texture::get_renderer_id() const
{
    return m_renderer_id;
}

int Texture::get_width() const
{
    return m_width;
}

int Texture::get_height() const
{
    return m_height;
}
