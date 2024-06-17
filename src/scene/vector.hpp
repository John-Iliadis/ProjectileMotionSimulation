//
// Created by Gianni on 16/06/2024.
//

#ifndef PROJECTILEMOTIONSIM_VECTOR_HPP
#define PROJECTILEMOTIONSIM_VECTOR_HPP

#include <cmath>
#include <imgui/imgui.h>
#include "../rendering/rendering_includes.hpp"


class Vector
{
public:
    Vector();
    Vector(const glm::vec2& pos, const glm::vec2& vel, float meter_as_pixels, const glm::vec4& color = glm::vec4(1));
    Vector(const glm::vec2& pos, float velocity, float angle /* degrees */, float meter_as_pixels, const glm::vec4& color = glm::vec4(1));

    void render();
    void set_position(const glm::vec2& position);
    void set_position(float x, float y);
    void set_velocity(const glm::vec2& velocity);
    void set_velocity(float x, float y);
    void set_meter_as_pixels(float meter_as_pixels);
    void set_view_proj_matrix(const glm::mat4& matrix);

private:
    glm::vec2 m_position;
    glm::vec2 m_velocity; // m/s
    glm::vec4 m_color;
    float m_meter_as_pixels;

    VertexArray m_vao;
    VertexBufferDynamic m_vbo;
    IndexBufferStatic m_ibo;
    Shader m_shader;
    const glm::mat4* m_view_proj;
    glm::mat4 m_model;
};


#endif //PROJECTILEMOTIONSIM_VECTOR_HPP
