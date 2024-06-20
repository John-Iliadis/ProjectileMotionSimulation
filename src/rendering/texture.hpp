//
// Created by Gianni on 20/06/2024.
//

#ifndef PROJECTILEMOTIONSIM_TEXTURE_HPP
#define PROJECTILEMOTIONSIM_TEXTURE_HPP

#include <stdexcept>
#include <glad/glad.h>
#include <stb/stb_image.h>


class Texture
{
public:
    Texture();
    Texture(const char* file_name);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void bind(uint32_t texture_unit = 0);
    void unbind() const;

    uint32_t get_renderer_id() const;
    int get_width() const;
    int get_height() const;

private:
    uint32_t m_renderer_id;
    int m_width;
    int m_height;
};


#endif //PROJECTILEMOTIONSIM_TEXTURE_HPP
