//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_FRAME_BUFFER_HPP
#define PROJECTILEMOTIONSIM_FRAME_BUFFER_HPP

#include <glad/glad.h>
#include <stdexcept>
#include <iostream>


class FrameBuffer
{
public:
    FrameBuffer();
    FrameBuffer(uint32_t texture_type, uint32_t width, uint32_t height);
    ~FrameBuffer();

    FrameBuffer(FrameBuffer&& other) noexcept;
    FrameBuffer& operator=(FrameBuffer&& other) noexcept;

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;

    void bind() const;
    void unbind() const;

    void resize(uint32_t width, uint32_t height);

    uint32_t get_framebuffer_id() const;
    uint32_t get_texture_id() const;
    uint32_t get_width() const;
    uint32_t get_height() const;

    static void gen_framebuffer(uint32_t& renderer_id, uint32_t& texture_id, uint32_t width, uint32_t height);
    static void gen_framebuffer_msaa(uint32_t& renderer_id, uint32_t& texture_id, uint32_t width, uint32_t height);

private:
    void create();
    void destroy();

private:
    uint32_t m_texture_type;
    uint32_t m_renderer_id;
    uint32_t m_texture_id;
    uint32_t m_width;
    uint32_t m_height;
};


#endif //PROJECTILEMOTIONSIM_FRAME_BUFFER_HPP
