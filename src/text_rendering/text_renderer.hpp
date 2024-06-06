//
// Created by Gianni on 2/03/2024.
//

#ifndef PROJECTILEMOTIONSIM_TEXT_RENDERER_HPP
#define PROJECTILEMOTIONSIM_TEXT_RENDERER_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <mutex>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../rendering/shader.hpp"
#include "../rendering/vertex_array.hpp"
#include "../rendering/vertex_buffer_dynamic.hpp"
#include "../rendering/index_buffer_dynamic.hpp"
#include "font_atlas.hpp"
#include "text.hpp"


// A text batch renderer
namespace TextRenderer
{
    void init_renderer();
    void draw_text(const Text& text);
    void render();
}


#endif //PROJECTILEMOTIONSIM_TEXT_RENDERER_HPP
