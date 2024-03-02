//
// Created by Gianni on 23/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_FONT_ATLAS_HPP
#define PROJECTILEMOTIONSIM_FONT_ATLAS_HPP

#include <string>
#include <map>
#include <stdexcept>
#include "glad/glad.h"
#include "glm/glm.hpp"


class FontAtlas
{
public:
    struct Character
    {
        glm::uvec2 size;
        glm::ivec2 bearing;
        glm::uvec2 tex_coords;
        uint32_t advance;
    };

public:
    FontAtlas();
    FontAtlas(const std::string& file_name, uint32_t font_size);
    ~FontAtlas();

    FontAtlas(FontAtlas&& other) noexcept;
    FontAtlas& operator=(FontAtlas&& other) noexcept;

    FontAtlas(const FontAtlas&) = delete;
    FontAtlas& operator=(const FontAtlas&) = delete;

    uint32_t get_texture_id() const;
    const Character& get_character(char c) const;
    const glm::uvec2& get_texture_size() const;

private:
    uint32_t m_texture_id;
    std::map<char, Character> m_characters;
    glm::uvec2 m_texture_size;
};


#endif //PROJECTILEMOTIONSIM_FONT_ATLAS_HPP
