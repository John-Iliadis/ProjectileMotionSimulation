//
// Created by Gianni on 16/06/2024.
//

#ifndef PROJECTILEMOTIONSIM_VECTOR_HPP
#define PROJECTILEMOTIONSIM_VECTOR_HPP

#include <cmath>
#include <imgui/imgui.h>
#include <memory>
#include <mutex>
#include "../opengl/includes.hpp"


class Vector
{
public:
    Vector();
    Vector(const glm::vec2& pos, const glm::vec2& magnitude, float meter_as_pixels, const glm::vec4& color = glm::vec4(1));
    Vector(const glm::vec2& pos, float magnitude, float angle /* degrees */, float meter_as_pixels, const glm::vec4& color = glm::vec4(1));

    void render();
    void set_position(const glm::vec2& position);
    void set_position(float x, float y);
    void set_magnitude(const glm::vec2& magnitude);
    void set_magnitude(float x, float y);
    void set_color_rgb(float r, float g, float b);
    void set_meter_as_pixels(float meter_as_pixels);
    void set_magnification(float magnification);

private:
    static void init();
    static void render_impl(const Vector& vector);

private:
    glm::vec2 m_position;
    glm::vec2 m_magnitude;
    glm::vec4 m_color;
    float m_meter_as_pixels;
    float m_magnification;

    VertexArray m_vao;
    mutable VertexBufferDynamic m_vbo;
};


#endif //PROJECTILEMOTIONSIM_VECTOR_HPP
