//
// Created by Gianni on 23/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_FONT_HPP
#define PROJECTILEMOTIONSIM_FONT_HPP

#include <string>
#include <unordered_map>
#include <stdexcept>
#include "glad/glad.h"
#include "glm/glm.hpp"


class Font
{
public:
    Font();
    Font(const std::string& file_name, uint32_t font_size);
    ~Font();

    Font(Font&& other) noexcept;
    Font& operator=(Font&& other) noexcept;

    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;

    uint32_t get_font_atlas() const;

private:
    uint32_t m_font_atlas_texture_id;
};


#endif //PROJECTILEMOTIONSIM_FONT_HPP
