//
// Created by Gianni on 20/06/2024.
//

#ifndef PROJECTILEMOTIONSIM_PROJECTILE_HPP
#define PROJECTILEMOTIONSIM_PROJECTILE_HPP

#include <memory>
#include <mutex>
#include <glm/glm.hpp>
#include "../rendering/rendering_includes.hpp"


class Projectile
{
public:
    Projectile(const glm::vec2& size, const glm::vec2& position);

    void render();
    void set_size(float width, float height);
    void set_size(const glm::vec2& size);
    void set_position(float x, float y);
    void set_position(const glm::vec2& position);

private:
    Texture m_texture;
    VertexArray m_vao;
    VertexBufferDynamic m_vbo;

    glm::vec2 m_size;
    glm::vec2 m_position;
};


#endif //PROJECTILEMOTIONSIM_PROJECTILE_HPP
