//
// Created by Gianni on 23/02/2024.
//

#include "font_atlas.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H


extern FT_Library ft_library;

FontAtlas::FontAtlas()
        : m_texture_id()
{
}

FontAtlas::FontAtlas(const std::string &file_name, uint32_t font_size)
{
    FT_Face face;
    if (FT_New_Face(ft_library, file_name.c_str(), 0, &face) != FT_Err_Ok)
    {
        throw std::runtime_error("Font::Font: Failed to load font: " + file_name + '\n');
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glm::uvec2 offset{};
    glm::uvec2 texture_size{};
    uint32_t max_glyph_height{};

    constexpr uint32_t max_texture_width = 1024;
    constexpr uint32_t char_start = 32;
    constexpr uint32_t char_end = 127;

    for (uint32_t i = char_start; i < char_end; ++i)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) != FT_Err_Ok)
        {
            throw std::runtime_error("Font::load_from_file: Failed to load glyph \"" + std::string(1, i) + "\"\n");
        }

        Character character {};

        character.size = {face->glyph->bitmap.width, face->glyph->bitmap.rows};
        character.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};
        character.advance = face->glyph->advance.x << 6;

        if (offset.x + character.size.x <= max_texture_width)
        {
            character.tex_coords = offset;
            offset.x += character.size.x;

            max_glyph_height = glm::max(max_glyph_height, character.size.y);
        }
        else
        {
            offset.x = 0;
            offset.y += max_glyph_height;

            max_glyph_height = character.size.y;

            character.tex_coords = offset;
            offset.x += character.size.x;
        }

        texture_size.x = glm::max(texture_size.x, offset.x);
        texture_size.y = glm::max(texture_size.y, offset.y + max_glyph_height);

        m_characters.insert(std::make_pair(i, std::move(character)));
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_size.x, texture_size.y, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (uint32_t i = char_start; i < char_end; ++i)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) != FT_Err_Ok)
        {
            throw std::runtime_error("Font::load_from_file: Failed to load glyph \"" + std::string(1, i) + "\"\n");
        }

        if (!face->glyph->bitmap.buffer)
            continue;

        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        m_characters.at(i).tex_coords.x,
                        m_characters.at(i).tex_coords.y,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
}

FontAtlas::~FontAtlas()
{
    glDeleteTextures(1, &m_texture_id);
}

FontAtlas::FontAtlas(FontAtlas &&other) noexcept
{
    m_texture_id = other.m_texture_id;
    m_characters = std::move(other.m_characters);
    other.m_texture_id = 0;
}

FontAtlas &FontAtlas::operator=(FontAtlas &&other) noexcept
{
    if (this != &other)
    {
        if (m_texture_id)
        {
            glDeleteTextures(1, &m_texture_id);
        }

        m_texture_id = other.m_texture_id;
        m_characters = std::move(other.m_characters);
        other.m_texture_id = 0;
    }

    return *this;
}

uint32_t FontAtlas::get_texture_id() const
{
    return m_texture_id;
}

const FontAtlas::Character &FontAtlas::get_character(char c) const
{
    auto itr = m_characters.find(c);

    if (itr != m_characters.end())
    {
        return itr->second;
    }
    else
    {
        throw std::runtime_error("FontAtlas::get_character: Character \"" + std::string(1, c) + "\" is missing from the map\n");
    }
}
