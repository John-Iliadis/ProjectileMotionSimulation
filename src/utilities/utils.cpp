//
// Created by Gianni on 19/02/2024.
//

#include <stb/stb_image_write.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utils.hpp"


namespace utils
{
    uint32_t gl_sizeof(uint32_t type)
    {
        switch (type)
        {
            case GL_FLOAT: return sizeof(float);
            case GL_UNSIGNED_INT: return sizeof(unsigned int);
            case GL_INT: return sizeof(int);
            case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
            case GL_BYTE: return sizeof(char);
            case GL_DOUBLE: return sizeof(double);
            default: throw std::runtime_error("utils::gl_sizeof: case not covered\n");
        }
    }

    void save_texture_as_png(const std::string& file_name, uint32_t texture_id)
    {
        int width, height, format;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);

        uint8_t* data;
        uint32_t channels{};

        switch (format)
        {
            case GL_RED:
                data = new uint8_t[width * height];
                channels = 1;
                break;

            case GL_RGB:
                data = new uint8_t[width * height * 3];
                channels = 3;
                break;

            default:
                throw std::runtime_error("utils::save_texture_as_png: Format not supported\n");
        }

        glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, data);

        stbi_flip_vertically_on_write(true);
        stbi_write_png(std::string(file_name + ".png").c_str(), width, height, channels, data, 0);

        delete[] data;
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glm::vec2 normalize_texture_coords(const glm::vec2& texture_coords, const glm::vec2& texture_size)
    {
        return texture_coords / texture_size;
    }
}
