//
// Created by Gianni on 2/03/2024.
//

#include "text_renderer.hpp"


static constexpr uint32_t MAX_QUADS = 20000;
static constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
static constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;
static int MAX_TEXTURES;

static std::unique_ptr<VertexArray> vao;
static std::unique_ptr<VertexBufferDynamic> vbo;
static std::unique_ptr<IndexBufferStatic> ibo;
static std::unique_ptr<Shader> text_shader;
static const glm::mat4* view_projection_matrix = nullptr;

static std::vector<TextVertex> vertices;
static std::vector<uint32_t> indices;
static std::unordered_map<const FontAtlas*, uint32_t> texture_unit_map;
static uint32_t index_count{};


namespace TextRenderer
{
    void init_renderer()
    {
        static std::once_flag flag;

        std::call_once(flag, [] ()
        {
            vertices.reserve(MAX_VERTICES);
            indices.reserve(MAX_INDICES);

            text_shader = std::make_unique<Shader>("../shaders/text.vert", "../shaders/text.frag");
            vao = std::make_unique<VertexArray>();
            vbo = std::make_unique<VertexBufferDynamic>(MAX_VERTICES * sizeof(TextVertex));

            VertexBufferLayout layout
            {
                {0, 2, GL_FLOAT, GL_FALSE}, // position
                {1, 2, GL_FLOAT, GL_FALSE}, // tex coordinates
                {2, 4, GL_FLOAT, GL_FALSE}, // color
                {3, 1, GL_FLOAT, GL_FALSE}  // texture slot
            };

            // setup quads indexes
            for (uint32_t i = 0, offset = 0; i < MAX_INDICES; i += 6, offset += 4)
            {
                indices.push_back(offset);
                indices.push_back(offset + 1);
                indices.push_back(offset + 2);

                indices.push_back(offset);
                indices.push_back(offset + 2);
                indices.push_back(offset + 3);
            }

            ibo = std::make_unique<IndexBufferStatic>(indices.data(), indices.size());

            vao->attach_vertex_buffer(*vbo, layout);
            vao->attach_index_buffer(*ibo);
            // todo: delete index vector

            // query for max number of textures
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURES);
        });
    }

    static void clear_batch()
    {
        vertices.clear();
        texture_unit_map.clear();
        index_count = 0;
    }

    static void bind_textures()
    {
        for (auto [font_atlas_ptr, slot_index] : texture_unit_map)
        {
            glActiveTexture(GL_TEXTURE0 + slot_index);
            glBindTexture(GL_TEXTURE_2D, font_atlas_ptr->get_texture_id());
        }
    }

    static void unbind_textures()
    {
        for (auto [font_atlas_ptr, slot_index] : texture_unit_map)
        {
            glActiveTexture(GL_TEXTURE0 + slot_index);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void pre_render(const OrthographicCamera& camera)
    {
        view_projection_matrix = &camera.get_view_projection_matrix();
    }

    void render()
    {
        // nothing to draw, so step out
        if (index_count == 0)
            return;

        // render here
        vbo->set_data(vertices.data(), vertices.size() * sizeof(TextVertex));
        bind_textures();
        text_shader->bind();
        text_shader->set_mat4("u_view_projection", *view_projection_matrix);
        vao->bind();

        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);

        unbind_textures();
        vao->unbind();
        text_shader->unbind();

        // clear all rendered data
        clear_batch();
    }

    // todo: don't apply kerning to first character of word
    void draw_text(const Text& text)
    {
        assert(text.font_atlas);

        // flush current batch if the max number of vertices will be surpassed in this function
        if (vertices.size() + text.string.size() * 4 >= MAX_VERTICES)
            render();

        // check if font texture is in the map
        if (!texture_unit_map.contains(text.font_atlas))
        {
            // the number of textures exceeds max textures, so flush current batch
            if (texture_unit_map.size() >= MAX_TEXTURES)
                render();

            uint32_t texture_unit_index = texture_unit_map.size();

            texture_unit_map.insert(std::make_pair(text.font_atlas, texture_unit_index));
        }

        uint32_t x_offset{};

        // create the vertices for every character in the string
        for (const auto c : text.string)
        {
            const FontAtlas::Character& character_details = text.font_atlas->get_character(c);

            // don't create vertices for space. Just advance the offset
            if (c == ' ')
            {
                x_offset += character_details.advance;
                continue;
            }

            glm::vec2 kerned_position
            {
                text.position.x + x_offset + character_details.bearing.x,
                text.position.y - (character_details.size.y - character_details.bearing.y)
            };

            const glm::uvec2& texture_coordinates = character_details.tex_coords;

            // the index of each vertex inside the vertices vector
            const size_t v1 = vertices.size();
            const size_t v2 = v1 + 1;
            const size_t v3 = v1 + 2;
            const size_t v4 = v1 + 3;

            // insert 4 vertices (quad) into the vector
            vertices.insert(vertices.end(), 4, TextVertex());

            // positions of quad vertices
            vertices.at(v1).position = kerned_position; // top left
            vertices.at(v2).position = {kerned_position.x + character_details.size.x, kerned_position.y}; // top right
            vertices.at(v3).position = {kerned_position.x + character_details.size.x, kerned_position.y + character_details.size.y}; // bottom right
            vertices.at(v4).position = {kerned_position.x, kerned_position.y + character_details.size.y}; // bottom left

            // texture coordinates of quad vertices
            vertices.at(v1).texture_coordinates = {texture_coordinates.x, texture_coordinates.y + character_details.size.y}; // top left
            vertices.at(v2).texture_coordinates = texture_coordinates + character_details.size; // top right
            vertices.at(v3).texture_coordinates = {texture_coordinates.x + character_details.size.x, texture_coordinates.y}; // bottom right
            vertices.at(v4).texture_coordinates = texture_coordinates; // bottom left

            // for each vertex, assign color, texture, and normalize texture coordinates.
            std::for_each(vertices.end() - 4, vertices.end(), [&text] (auto& vertex)
            {
                vertex.color = text.color;
                vertex.texture_unit_index = texture_unit_map.at(text.font_atlas);
                vertex.texture_coordinates = utils::normalize_texture_coords(vertex.texture_coordinates, text.font_atlas->get_texture_size());
            });

            index_count += 6;
            x_offset += character_details.advance;
        }
    }
}
