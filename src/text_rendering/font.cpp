//
// Created by Gianni on 23/02/2024.
//

#include "font.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H


extern FT_Library ft_library;

Font::Font()
    : m_font_atlas_texture_id()
{
}

Font::Font(const std::string &file_name, uint32_t font_size)
{
    FT_Face face;
    if (FT_New_Face(ft_library, file_name.c_str(), 0, &face) != FT_Err_Ok)
    {
        throw std::runtime_error("Font::Font: Failed to load font: " + file_name + '\n');
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    uint32_t max_width{};
    uint32_t max_height{};
    uint32_t char_start = 32;
    uint32_t char_end = 127;

    for (uint32_t i = char_start; i < char_end; ++i)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) != FT_Err_Ok)
        {
            throw std::runtime_error("Font::load_from_file: Failed to load glyph \"" + std::string(1, i) + "\"\n");
        }

        max_width += face->glyph->bitmap.width;
        max_height = glm::max(max_height, face->glyph->bitmap.rows);
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_font_atlas_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_font_atlas_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, max_width, max_height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (uint32_t i = char_start, offset = 0; i < char_end; ++i, offset += face->glyph->bitmap.width)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) != FT_Err_Ok)
        {
            throw std::runtime_error("Font::load_from_file: Failed to load glyph \"" + std::string(1, i) + "\"\n");
        }

        if (!face->glyph->bitmap.buffer)
            continue;

        glTexSubImage2D(GL_TEXTURE_2D, 0, offset, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
}

Font::~Font()
{
    glDeleteTextures(1, &m_font_atlas_texture_id);
}

Font::Font(Font &&other) noexcept
{
    m_font_atlas_texture_id = other.m_font_atlas_texture_id;
    other.m_font_atlas_texture_id = 0;
}

Font &Font::operator=(Font &&other) noexcept
{
    if (this != &other)
    {
        if (m_font_atlas_texture_id)
        {
            glDeleteTextures(1, &m_font_atlas_texture_id);
        }

        m_font_atlas_texture_id = other.m_font_atlas_texture_id;
        other.m_font_atlas_texture_id = 0;
    }

    return *this;
}

uint32_t Font::get_font_atlas() const
{
    return m_font_atlas_texture_id;
}
