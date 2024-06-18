//
// Created by Gianni on 18/06/2024.
//

#ifndef PROJECTILEMOTIONSIM_UNIFORM_BUFFER_OBJECT_DYNAMIC_HPP
#define PROJECTILEMOTIONSIM_UNIFORM_BUFFER_OBJECT_DYNAMIC_HPP

#include <cstdint>
#include <glad/glad.h>
#include "vertex_buffer.hpp"


class UniformBufferObjectDynamic : public VertexBuffer
{
public:
    UniformBufferObjectDynamic();
    UniformBufferObjectDynamic(const uint32_t buffer_size, const uint32_t index);
    ~UniformBufferObjectDynamic();

    UniformBufferObjectDynamic(const UniformBufferObjectDynamic&) = delete;
    UniformBufferObjectDynamic& operator=(const UniformBufferObjectDynamic&) = delete;

    UniformBufferObjectDynamic(UniformBufferObjectDynamic&& other) noexcept;
    UniformBufferObjectDynamic& operator=(UniformBufferObjectDynamic&& other) noexcept;

    void set_data(const void* data, const uint32_t buffer_size);

    void bind() const override;
    void unbind() const override;

    uint32_t get_index() const;

private:
    uint32_t m_renderer_id;
    uint32_t m_index;
};


#endif //PROJECTILEMOTIONSIM_UNIFORM_BUFFER_OBJECT_DYNAMIC_HPP
