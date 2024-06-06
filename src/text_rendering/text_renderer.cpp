//
// Created by Gianni on 2/03/2024.
//

#include "text_renderer.hpp"


static constexpr uint32_t max_quads = 20000;
static constexpr uint32_t max_vertices = max_quads * 4;
static constexpr uint32_t max_indices = max_quads * 6;
static constexpr uint32_t max_textures = 32; // todo query for max textures

static std::unique_ptr<Shader> text_shader;
static std::unique_ptr<VertexArray> vao;
static std::unique_ptr<VertexBufferDynamic> vbo;
static std::unique_ptr<IndexBufferStatic> ibo;

static std::vector<TextVertex> text_vertices;
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
            text_shader = std::make_unique<Shader>("../shaders/text.vert", "../shaders/text.frag");
            vao = std::make_unique<VertexArray>();
            vbo = std::make_unique<VertexBufferDynamic>(max_vertices * sizeof(TextVertex));

            VertexBufferLayout layout {
                    {0, 2, GL_FLOAT, GL_FALSE},
                    {1, 2, GL_FLOAT, GL_FALSE},
                    {2, 4, GL_FLOAT, GL_FALSE},
                    {3, 1, GL_FLOAT, GL_FALSE}
            };

            // todo: check capacity
            // setup quads indexes
            indices.reserve(max_indices);
            for (uint32_t i = 0, offset = 0; i < max_indices; i += 6, offset += 4)
            {
                indices.push_back(offset);
                indices.push_back(offset + 1);
                indices.push_back(offset + 2);

                indices.push_back(offset + 2);
                indices.push_back(offset + 3);
                indices.push_back(offset);
            }

            ibo = std::make_unique<IndexBufferStatic>(indices.data(), indices.size());

            vao->attach_vertex_buffer(*vbo, layout);
            vao->attach_index_buffer(*ibo);

            // todo: delete index vector
        });
    }

    static void clear_batch()
    {
        text_vertices.clear();
        texture_unit_map.clear();
        index_count = 0;
    }

    void render()
    {
        // render here
        clear_batch();
    }

    void draw_text(const Text& text)
    {
        // todo: check if current batch is full

        assert(text.font_atlas);

        if (!texture_unit_map.contains(text.font_atlas))
        {
            uint32_t texture_unit_index = texture_unit_map.size();

            texture_unit_map.insert(std::make_pair(text.font_atlas, texture_unit_index));
        }

        uint32_t x_offset{};

        for (auto c = text.string.begin(); c != text.string.end(); ++c)
        {
            const FontAtlas::Character& character_details = text.font_atlas->get_character(*c);

            std::array<TextVertex, 4> quad;

            glm::vec2 kerned_position {
                text.position.x + x_offset + character_details.bearing.x,
                text.position.y - (character_details.size.x - character_details.bearing.y)
            };

            glm::uvec2 texture_coordinates {
                    character_details.tex_coords.x,
                    text.font_atlas->get_texture_size().y - character_details.tex_coords.y + character_details.size.y
            };

            quad[0].position = kerned_position;
            quad[1].position = {kerned_position.x, kerned_position.y + character_details.size.y};
            quad[2].position = {kerned_position.x + character_details.size.x, kerned_position.y + character_details.size.y};
            quad[3].position = {kerned_position.x + character_details.size.x, kerned_position.y};

            quad[0].texture_coordinates = texture_coordinates;
            quad[1].texture_coordinates = {texture_coordinates.x, texture_coordinates.y + character_details.size.y};
            quad[2].texture_coordinates = texture_coordinates + character_details.size;
            quad[3].texture_coordinates = {texture_coordinates.x + character_details.size.x, texture_coordinates.y};

            std::for_each(quad.begin(), quad.end(), [&text] (auto& vertex) {
                vertex.color = text.color;
                vertex.texture_unit_index = texture_unit_map.at(text.font_atlas);
                vertex.texture_coordinates = utils::normalize_texture_coords(vertex.texture_coordinates, text.font_atlas->get_texture_size());
            });

            // todo: append data to vertices
            // todo: check that normalized coordinates are correct
            // todo: implement finish render
            // todo: implement flush function
            // todo: query for max texture slots

            index_count += 6;
            x_offset += character_details.advance;
        }
    }
}
