//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_VERTEX_BUFFER_LAYOUT_HPP
#define PROJECTILEMOTIONSIM_VERTEX_BUFFER_LAYOUT_HPP

#include <glad/glad.h>
#include <vector>
#include "../utilities/utils.hpp"


class VertexBufferLayout
{
public:
    struct attribute
    {
        uint32_t index;
        uint32_t size;
        uint32_t type;
        uint32_t normalized;
    };

public:
    VertexBufferLayout();
    VertexBufferLayout(const std::vector<attribute>& attributes);
    VertexBufferLayout(std::initializer_list<attribute> attributes);

    void add_attribute(const attribute& attribute);
    void add_attribute(attribute&& attribute);

    uint32_t get_stride() const;
    const std::vector<attribute>& get_attributes() const;

private:
    void increase_stride(const attribute& attribute);

private:
    std::vector<attribute> m_attributes;
    uint32_t m_stride;
};


#endif //PROJECTILEMOTIONSIM_VERTEX_BUFFER_LAYOUT_HPP
