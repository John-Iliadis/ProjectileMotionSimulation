//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_VERTEX_ARRAY_HPP
#define PROJECTILEMOTIONSIM_VERTEX_ARRAY_HPP

#include "vertex_buffer_static.hpp"
#include "vertex_buffer_dynamic.hpp"
#include "index_buffer_static.hpp"
#include "vertex_buffer_layout.hpp"


class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    void bind() const;
    void unbind() const;

    void attach_vertex_buffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
    void attach_index_buffer(const IndexBuffer& ibo);

private:
    uint32_t m_renderer_id;
};


#endif //PROJECTILEMOTIONSIM_VERTEX_ARRAY_HPP
