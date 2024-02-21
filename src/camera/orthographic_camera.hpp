//
// Created by Gianni on 21/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_ORTHOGRAPHIC_CAMERA_HPP
#define PROJECTILEMOTIONSIM_ORTHOGRAPHIC_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class OrthographicCamera
{
public:
    OrthographicCamera();
    OrthographicCamera(float left, float right, float bottom, float top, float near = -1, float far = 1);

    void set_projection(float left, float right, float bottom, float top, float near = -1, float far = 1);

    void set_position(const glm::vec3& position);
    void set_position(float x, float y, float z);

    const glm::vec3& get_position() const;
    const glm::mat4& get_projection_matrix() const;
    const glm::mat4& get_view_matrix() const;
    const glm::mat4& get_view_projection_matrix() const;

private:
    void recalculate_view_matrix();

private:
    glm::mat4 m_projection_matrix;
    glm::mat4 m_view_matrix;
    glm::mat4 m_view_projection_matrix;

    glm::vec3 m_position;
};


#endif //PROJECTILEMOTIONSIM_ORTHOGRAPHIC_CAMERA_HPP
