//
// Created by Gianni on 2/03/2024.
//

#ifndef PROJECTILEMOTIONSIM_TEXT_HPP
#define PROJECTILEMOTIONSIM_TEXT_HPP

#include <string>
#include <glm/glm.hpp>


class FontAtlas;

struct TextVertex
{
    glm::vec2 position;
    glm::vec2 texture_coordinates;
    glm::vec4 color;
    uint32_t texture_unit;

    TextVertex()
        : position()
        , texture_coordinates()
        , color(1)
        , texture_unit(-1)
    {
    }
};

struct Text
{
    const FontAtlas* font_atlas = nullptr;
    std::string text;
    glm::vec2 position;
    glm::vec4 color;

    Text()
        : font_atlas()
        , text()
        , position()
        , color(1)
    {
    }
};


#endif //PROJECTILEMOTIONSIM_TEXT_HPP
