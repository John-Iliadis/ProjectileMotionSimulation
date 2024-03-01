//
// Created by Gianni on 2/03/2024.
//

#ifndef PROJECTILEMOTIONSIM_VERTEX_BUFFER_HPP
#define PROJECTILEMOTIONSIM_VERTEX_BUFFER_HPP


class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
};


#endif //PROJECTILEMOTIONSIM_VERTEX_BUFFER_HPP
