//
// Created by Gianni on 2/03/2024.
//

#ifndef PROJECTILEMOTIONSIM_RENDERER2D_HPP
#define PROJECTILEMOTIONSIM_RENDERER2D_HPP

#include <string>
#include <memory>
#include <array>
#include <algorithm>
#include <mutex>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../rendering/shader.hpp"
#include "../rendering/vertex_array.hpp"
#include "../rendering/vertex_buffer_dynamic.hpp"
#include "../rendering/index_buffer_dynamic.hpp"
#include "../camera/orthographic_camera.hpp"
#include "../simulation/vector.hpp"
#include "renderer2D_constants.hpp"
#include "font_atlas.hpp"
#include "text.hpp"


// batch renderer
namespace renderer2D
{
    void init_renderer();
    void render();

    void draw_point(float x, float y, const glm::vec4& color = glm::vec4(1));
    void draw_point(const glm::vec2& pos, const glm::vec4& color = glm::vec4(1));

    void draw_line(float x1, float y1, float x2, float y2, const glm::vec4& color = glm::vec4(1));
    void draw_line(const glm::vec2& point_1, const glm::vec2& point_2, const glm::vec4& color = glm::vec4(1));

    void draw_text(const Text& text);
}


#endif //PROJECTILEMOTIONSIM_RENDERER2D_HPP
