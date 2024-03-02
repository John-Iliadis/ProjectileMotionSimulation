//
// Created by Gianni on 2/03/2024.
//

#ifndef PROJECTILEMOTIONSIM_INDEX_BUFFER_DYNAMIC_HPP
#define PROJECTILEMOTIONSIM_INDEX_BUFFER_DYNAMIC_HPP

#include <glad/glad.h>
#include "index_buffer.hpp"


class IndexBufferDynamic : public IndexBuffer
{
public:
    IndexBufferDynamic();
    IndexBufferDynamic(size_t size);
    ~IndexBufferDynamic();

    IndexBufferDynamic(IndexBufferDynamic&& other) noexcept;
    IndexBufferDynamic& operator=(IndexBufferDynamic&& other) noexcept;

    IndexBufferDynamic(const IndexBufferDynamic&) = delete;
    IndexBufferDynamic& operator=(const IndexBufferDynamic&) = delete;

    void set_data(const void* data, uint32_t count);

    void bind() const override;
    void unbind() const override;

private:
    uint32_t m_renderer_id;
};


#endif //PROJECTILEMOTIONSIM_INDEX_BUFFER_DYNAMIC_HPP
