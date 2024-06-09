//
// Created by Gianni on 2/03/2024.
//

#ifndef PROJECTILEMOTIONSIM_TEXT_HPP
#define PROJECTILEMOTIONSIM_TEXT_HPP

#include <string>
#include <glm/glm.hpp>


class FontAtlas;

enum class TextOrigin
{
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT,
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER,
    CENTER_RIGHT
};

// A text object used for rendering
struct Text
{
    const FontAtlas* font_atlas;
    std::string string;
    glm::vec2 position;
    TextOrigin origin = TextOrigin::BOTTOM_LEFT;
    glm::vec4 color = glm::vec4(1);
};


struct TextVertex
{
    glm::vec2 position;
    glm::vec2 texture_coordinates;
    glm::vec4 color = glm::vec4(1);
    uint32_t texture_unit_index;  // specifies the texture font atlas
};


#endif //PROJECTILEMOTIONSIM_TEXT_HPP
