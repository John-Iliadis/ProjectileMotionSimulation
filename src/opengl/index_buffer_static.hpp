//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_INDEX_BUFFER_STATIC_HPP
#define PROJECTILEMOTIONSIM_INDEX_BUFFER_STATIC_HPP

#include <glad/glad.h>
#include "index_buffer.hpp"


class IndexBufferStatic : public IndexBuffer
{
public:
    IndexBufferStatic();
    IndexBufferStatic(const void* data, uint32_t count);
    ~IndexBufferStatic();

    IndexBufferStatic(IndexBufferStatic&& other) noexcept;
    IndexBufferStatic& operator=(IndexBufferStatic&& other) noexcept;

    IndexBufferStatic(const IndexBufferStatic&) = delete;
    IndexBufferStatic& operator=(const IndexBufferStatic&) = delete;

    void bind() const override;
    void unbind() const override;

    uint32_t get_count() const;

private:
    uint32_t m_renderer_id;
    uint32_t m_count;
};


#endif //PROJECTILEMOTIONSIM_INDEX_BUFFER_STATIC_HPP
