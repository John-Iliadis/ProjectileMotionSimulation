//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_VERTEX_BUFFER_STATIC_HPP
#define PROJECTILEMOTIONSIM_VERTEX_BUFFER_STATIC_HPP

#include <glad/glad.h>


class VertexBufferStatic
{
public:
    VertexBufferStatic();
    VertexBufferStatic(const void* data, uint32_t count);
    ~VertexBufferStatic();

    VertexBufferStatic(VertexBufferStatic&& other) noexcept;
    VertexBufferStatic& operator=(VertexBufferStatic&& other) noexcept;

    VertexBufferStatic(const VertexBufferStatic&) = delete;
    VertexBufferStatic& operator=(const VertexBufferStatic&) = delete;

    void bind() const;
    void unbind() const;

    uint32_t get_count() const;

private:
    uint32_t m_renderer_id;
    uint32_t m_count;
};


#endif //PROJECTILEMOTIONSIM_VERTEX_BUFFER_STATIC_HPP
