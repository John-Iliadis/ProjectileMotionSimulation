//
// Created by Gianni on 2/03/2024.
//

#ifndef PROJECTILEMOTIONSIM_INDEX_BUFFER_HPP
#define PROJECTILEMOTIONSIM_INDEX_BUFFER_HPP


class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
};


#endif //PROJECTILEMOTIONSIM_INDEX_BUFFER_HPP
