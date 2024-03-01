//
// Created by Gianni on 2/03/2024.
//

#ifndef PROJECTILEMOTIONSIM_VERTEX_BUFFER_DYNAMIC_HPP
#define PROJECTILEMOTIONSIM_VERTEX_BUFFER_DYNAMIC_HPP

#include <glad/glad.h>
#include "vertex_buffer.hpp"


class VertexBufferDynamic : public VertexBuffer
{
public:
    VertexBufferDynamic();
    VertexBufferDynamic(size_t buffer_size);
    ~VertexBufferDynamic();

    VertexBufferDynamic(VertexBufferDynamic&& other) noexcept;
    VertexBufferDynamic& operator=(VertexBufferDynamic&& other) noexcept;

    VertexBufferDynamic(const VertexBufferDynamic&) = delete;
    VertexBufferDynamic& operator=(const VertexBufferDynamic&) = delete;

    void set_data(const void* data, uint32_t count);

    void bind() const override;
    void unbind() const override;

private:
    uint32_t m_renderer_id;
};


#endif //PROJECTILEMOTIONSIM_VERTEX_BUFFER_DYNAMIC_HPP
