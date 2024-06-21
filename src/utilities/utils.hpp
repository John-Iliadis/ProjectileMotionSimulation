//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_UTILS_HPP
#define PROJECTILEMOTIONSIM_UTILS_HPP

#include <stdexcept>
#include <array>
#include <glad/glad.h>
#include <stb/stb_image_write.h>
#include <glm/glm.hpp>


namespace utils
{
    uint32_t gl_sizeof(uint32_t type);

    void save_texture_as_png(const std::string& file_name, uint32_t texture_id);

    glm::vec2 normalize_texture_coords(const glm::vec2& texture_coords, const glm::vec2& texture_size);

    float poly4_root(const std::array<float, 5>& poly4, const uint32_t iterations = 10);
};


#endif //PROJECTILEMOTIONSIM_UTILS_HPP
