//
// Created by Gianni on 20/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_SCENE_HPP
#define PROJECTILEMOTIONSIM_SCENE_HPP

#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include "../rendering/rendering_includes.hpp"
#include "../camera/orthographic_camera.hpp"


class Scene
{
public:
    Scene(uint32_t width, uint32_t height);

    void update(double dt);
    void render();

private:
    void pre_render();
    void post_render();
    void resize_scene(uint32_t width, uint32_t height);

private:
    FrameBuffer m_fbo;
    FrameBuffer m_intermediate_fbo;
    Shader m_shader;
    OrthographicCamera m_camera;
    VertexArray vao;
    VertexBufferStatic vbo;
    uint32_t m_width;
    uint32_t m_height;

    std::vector<float> vertices;
};


#endif //PROJECTILEMOTIONSIM_SCENE_HPP
