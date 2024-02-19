//
// Created by Gianni on 19/02/2024.
//

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
            default: throw std::runtime_error("gl_sizeof: case not covered\n");
        }
    }
}
