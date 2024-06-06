//
// Created by Gianni on 22/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_GRAPH_HPP
#define PROJECTILEMOTIONSIM_GRAPH_HPP

#include <array>
#include <vector>
#include <glad/glad.h>
#include "../rendering/vertex_array.hpp"
#include "../rendering/vertex_buffer_static.hpp"
#include "../rendering/vertex_buffer_layout.hpp"
#include "../rendering/shader.hpp"


// The graph of the projectile motion simulation
class Graph
{
public:
    Graph();

    void create(float width, float height);
    void render();

private:
    void create_axes(float width, float height, float origin_offset);
    void create_points(float width, float height, float origin_offset);

private:
    std::vector<float> m_vertices;
    VertexArray m_vao;
    VertexBufferStatic m_vbo;

    float m_width;
    float m_height;
};


#endif //PROJECTILEMOTIONSIM_GRAPH_HPP
