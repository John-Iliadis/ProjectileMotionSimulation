//
// Created by Gianni on 18/06/2024.
//

#ifndef PROJECTILEMOTIONSIM_RENDERER2D_CONSTANTS_HPP
#define PROJECTILEMOTIONSIM_RENDERER2D_CONSTANTS_HPP

#include <cstdint>


namespace renderer2D::constants
{
    // quads
    inline constexpr uint32_t QUAD_VERTEX_COUNT = 4;
    inline constexpr uint32_t QUAD_INDEX_COUNT = 6;
    inline constexpr uint32_t MAX_QUADS = 20'000;
    inline constexpr uint32_t MAX_QUAD_VERTICES = MAX_QUADS * QUAD_VERTEX_COUNT;
    inline constexpr uint32_t MAX_QUAD_INDICES = MAX_QUADS * QUAD_INDEX_COUNT;

    // points
    inline constexpr uint32_t MAX_POINTS = 20'000;
    inline constexpr uint32_t MAX_POINT_VERTICES = MAX_POINTS;

    // lines
    inline constexpr uint32_t LINE_VERTEX_COUNT = 2;
    inline constexpr uint32_t MAX_LINES = 20'000;
    inline constexpr uint32_t MAX_LINE_VERTICES = MAX_LINES * LINE_VERTEX_COUNT;
}


#endif //PROJECTILEMOTIONSIM_RENDERER2D_CONSTANTS_HPP
