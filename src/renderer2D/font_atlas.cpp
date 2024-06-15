//
// Created by Gianni on 23/02/2024.
//

#include "font_atlas.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H

#include <iostream>

extern FT_Library ft_library;

FontAtlas::FontAtlas()
    : m_texture_id()
    , m_characters()
    , m_texture_size()
{
}

FontAtlas::FontAtlas(const std::string &file_name, uint32_t font_size)
    : m_texture_id()
    , m_characters()
    , m_texture_size()
{
    FT_Face face;
    if (FT_New_Face(ft_library, file_name.c_str(), 0, &face) != FT_Err_Ok)
    {
        throw std::runtime_error("Font::Font: Failed to load font: " + file_name + '\n');
    }

    FT_Set_Pixel_Sizes(face, 0, font_size); // set glyph size

    int previous_alignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glm::uvec2 offset{}; // the offset of the current glyph in the texture
    uint32_t max_glyph_height{};

    constexpr uint32_t MAX_TEXTURE_WIDTH = 1024;
    constexpr uint8_t CHAR_START = 32;
    constexpr uint8_t CHAR_END = 127;

    for (uint8_t i = CHAR_START; i < CHAR_END; ++i)
    {
        // load a glyph
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) != FT_Err_Ok)
        {
            throw std::runtime_error("Font::load_from_file: Failed to load glyph \"" + std::string(1, i) + "\"\n");
        }

        Character character {};

        character.size = {face->glyph->bitmap.width, face->glyph->bitmap.rows};
        character.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};
        character.advance = face->glyph->advance.x >> 6;

        assert(character.size.x <= MAX_TEXTURE_WIDTH);

        // place the glyph in the current row of the texture if the texture width remains lower than the max texture
        // width. Else, place glyph on a new row below.
        if (offset.x + character.size.x <= MAX_TEXTURE_WIDTH)
        {
            character.tex_coords = offset;
            offset.x += character.size.x + 2; // there is a one pixel margin between glyphs

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

        m_texture_size.x = glm::max(m_texture_size.x, offset.x);
        m_texture_size.y = glm::max(m_texture_size.y, offset.y + max_glyph_height);

        auto result = m_characters.insert(std::make_pair(i, std::move(character)));

        assert(result.second);
    }

    // the following process creates the texture font atlas object
    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_texture_size.x, m_texture_size.y, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (uint8_t i = CHAR_START; i < CHAR_END; ++i)
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
    glPixelStorei(GL_UNPACK_ALIGNMENT, previous_alignment);

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
    m_texture_size = std::move(other.m_texture_size);
    other.m_texture_id = 0;
}

FontAtlas& FontAtlas::operator=(FontAtlas &&other) noexcept
{
    if (this != &other)
    {
        if (m_texture_id)
        {
            glDeleteTextures(1, &m_texture_id);
        }

        m_texture_id = other.m_texture_id;
        m_characters = std::move(other.m_characters);
        m_texture_size = std::move(other.m_texture_size);
        other.m_texture_id = 0;
    }

    return *this;
}

uint32_t FontAtlas::get_texture_id() const
{
    return m_texture_id;
}

const FontAtlas::Character& FontAtlas::get_character(char c) const
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

const glm::uvec2& FontAtlas::get_texture_size() const
{
    return m_texture_size;
}
