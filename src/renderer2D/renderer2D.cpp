//
// Created by Gianni on 2/03/2024.
//

#include "renderer2D.hpp"


using namespace renderer2D::constants;

struct PointVertex
{
    glm::vec2 position;
    glm::vec4 color;
};

struct TextVertex
{
    glm::vec2 position;
    glm::vec2 texture_coordinates;
    uint32_t texture_unit_index;
    glm::vec4 color;
};


// point buffers
static std::unique_ptr<VertexArray> points_vao;
static std::unique_ptr<VertexBufferDynamic> points_vbo;
static std::unique_ptr<Shader> point_line_shader;
static std::vector<PointVertex> point_vertices;

// line buffers
static std::unique_ptr<VertexArray> lines_vao;
static std::unique_ptr<VertexBufferDynamic> lines_vbo;
static std::vector<PointVertex> line_vertices;

// text buffers
static std::unique_ptr<VertexArray> text_vao;
static std::unique_ptr<VertexBufferDynamic> text_vbo;
static std::unique_ptr<Shader> text_shader;
static std::vector<TextVertex> text_vertices;
static uint32_t text_index_count = 0;

// general
static int max_textures;
static std::unique_ptr<IndexBufferStatic> quads_ibo;
static std::vector<uint32_t> quad_indices;
static std::vector<uint32_t> textures;

static glm::vec2 get_text_dimensions(const Text& text);
static glm::vec2 calculate_text_position(const Text& text);


namespace renderer2D
{
    void init_renderer()
    {
        static std::once_flag flag;

        std::call_once(flag, [] ()
        {
            point_vertices.reserve(MAX_POINT_VERTICES);
            line_vertices.reserve(MAX_LINE_VERTICES);
            text_vertices.reserve(MAX_QUAD_VERTICES);
            quad_indices.reserve(MAX_QUAD_INDICES);

            // setup quads indexes
            for (uint32_t i = 0, offset = 0; i < MAX_QUAD_INDICES; i += QUAD_INDEX_COUNT, offset += QUAD_VERTEX_COUNT)
            {
                quad_indices.push_back(offset);
                quad_indices.push_back(offset + 1);
                quad_indices.push_back(offset + 2);

                quad_indices.push_back(offset);
                quad_indices.push_back(offset + 2);
                quad_indices.push_back(offset + 3);
            }

            quads_ibo = std::make_unique<IndexBufferStatic>(quad_indices.data(), quad_indices.size());
            std::vector<uint32_t>().swap(quad_indices); // delete indices memory since they're copied in vram

            // points setup
            glPointSize(5);
            point_line_shader = std::make_unique<Shader>("../shaders/point_line.vert", "../shaders/point_line.frag");
            points_vao = std::make_unique<VertexArray>();
            points_vbo = std::make_unique<VertexBufferDynamic>(MAX_POINT_VERTICES * sizeof(PointVertex));

            VertexBufferLayout point_line_vertex_layout
            {
                {0, 2, GL_FLOAT, GL_FALSE}, // position
                {1, 4, GL_FLOAT, GL_FALSE}  // color
            };

            points_vao->attach_vertex_buffer(*points_vbo, point_line_vertex_layout);

            // lines setup
            lines_vao = std::make_unique<VertexArray>();
            lines_vbo = std::make_unique<VertexBufferDynamic>(MAX_LINE_VERTICES * sizeof(PointVertex));
            lines_vao->attach_vertex_buffer(*lines_vbo, point_line_vertex_layout);

            // text setup
            text_shader = std::make_unique<Shader>("../shaders/text.vert", "../shaders/text.frag");
            text_vao = std::make_unique<VertexArray>();
            text_vbo = std::make_unique<VertexBufferDynamic>(MAX_QUAD_VERTICES * sizeof(TextVertex));

            VertexBufferLayout layout
            {
                {0, 2, GL_FLOAT, GL_FALSE}, // position
                {1, 2, GL_FLOAT, GL_FALSE}, // tex coordinates
                {2, 4, GL_FLOAT, GL_FALSE}, // color
                {3, 1, GL_FLOAT, GL_FALSE}  // texture slot
            };

            text_vao->attach_vertex_buffer(*text_vbo, layout);
            text_vao->attach_index_buffer(*quads_ibo);

            // query for max number of textures
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_textures);
            textures.reserve(max_textures);
        });
    }

    static void clear_batch()
    {
        point_vertices.clear();
        line_vertices.clear();
        text_vertices.clear();
        textures.clear();
        text_index_count = 0;
    }

    static void bind_textures()
    {
        for (int i = 0; i < textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures.at(i));
        }
    }

    static void unbind_textures()
    {
        for (int i = 0; i < textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    static void render_points()
    {
        if (point_vertices.size())
        {
            points_vbo->set_data(point_vertices.data(), point_vertices.size() * sizeof(PointVertex));
            point_line_shader->bind();
            points_vao->bind();

            glDrawArrays(GL_POINTS, 0, point_vertices.size());

            points_vao->unbind();
            point_line_shader->unbind();
        }
    }

    static void render_lines()
    {
        if (line_vertices.size())
        {
            lines_vbo->set_data(line_vertices.data(), line_vertices.size() * sizeof(PointVertex));
            point_line_shader->bind();
            lines_vao->bind();

            glDrawArrays(GL_LINES, 0, line_vertices.size());

            lines_vao->unbind();
            point_line_shader->unbind();
        }
    }

    static void render_text()
    {
        if (text_vertices.size())
        {
            text_vbo->set_data(text_vertices.data(), text_vertices.size() * sizeof(TextVertex));
            bind_textures();
            text_shader->bind();
            text_vao->bind();

            glDrawElements(GL_TRIANGLES, text_index_count, GL_UNSIGNED_INT, nullptr);

            unbind_textures();
            text_vao->unbind();
            text_shader->unbind();
        }
    }

    void render()
    {
        render_text();
        render_lines();
        render_points();

        clear_batch();
    }

    void draw_point(float x, float y, const glm::vec4& color)
    {
        if (point_vertices.size() == MAX_POINT_VERTICES)
            render_points();

        point_vertices.push_back({{x, y}, color});
    }

    void draw_point(const glm::vec2& pos, const glm::vec4& color)
    {
        draw_point(pos.x, pos.y, color);
    }

    void draw_line(float x1, float y1, float x2, float y2, const glm::vec4& color)
    {
        if (line_vertices.size() + 2 > MAX_LINE_VERTICES)
            render_lines();

        line_vertices.push_back({{x1, y1}, color});
        line_vertices.push_back({{x2, y2}, color});
    }

    void draw_line(const glm::vec2& point_1, const glm::vec2& point_2, const glm::vec4& color)
    {
        draw_line(point_1.x, point_1.y, point_2.x, point_2.y, color);
    }

    void draw_text(const Text& text)
    {
        assert(text.font_atlas);

        // flush current batch if the max number of vertices will be surpassed in this function
        if (text_vertices.size() + text.string.size() * 4 >= MAX_QUAD_VERTICES)
            render();

        // check if font texture is in textures
        size_t texture_unit_index;
        auto itr = std::find(textures.begin(), textures.end(), text.font_atlas->get_texture_id());
        if (itr == textures.end())
        {
            // the number of textures is at max textures, so flush current batch
            if (textures.size() == max_textures)
                render();

            textures.push_back(text.font_atlas->get_texture_id());
            texture_unit_index = textures.size() - 1;
        }
        else
        {
            texture_unit_index = std::distance(itr, textures.end());
        }

        uint32_t x_offset{};
        glm::vec2 text_origin = calculate_text_position(text);

        // create the vertices for every character in the string
        for (const char c : text.string)
        {
            const FontAtlas::Character& char_details = text.font_atlas->get_character(c);

            // don't create vertices for space. Just advance the offset
            if (c == ' ')
            {
                x_offset += char_details.advance;
                continue;
            }

            glm::vec2 pos
            {
                text_origin.x + static_cast<float>(x_offset) + static_cast<float>(char_details.bearing.x),
                text_origin.y - (static_cast<float>(char_details.size.y) - static_cast<float>(char_details.bearing.y))
            };

            const glm::uvec2& tex_coords = char_details.tex_coords;

            // the index of each vertex inside the vertices vector
            const size_t v1 = text_vertices.size();
            const size_t v2 = v1 + 1;
            const size_t v3 = v1 + 2;
            const size_t v4 = v1 + 3;

            // insert 4 vertices (quad) into the vector
            text_vertices.insert(text_vertices.end(), 4, TextVertex());

            // positions of quad vertices
            text_vertices.at(v1).position = {pos.x, pos.y}; // bottom left
            text_vertices.at(v2).position = {pos.x, pos.y + char_details.size.y}; // top left
            text_vertices.at(v3).position = {pos.x + char_details.size.x, pos.y + char_details.size.y}; // top right
            text_vertices.at(v4).position = {pos.x + char_details.size.x, pos.y}; // bottom right

            // texture coordinates of quad vertices. The y-axis is flipped
            text_vertices.at(v1).texture_coordinates = {tex_coords.x, tex_coords.y + char_details.size.y};
            text_vertices.at(v2).texture_coordinates = {tex_coords.x, tex_coords.y};
            text_vertices.at(v3).texture_coordinates = {tex_coords.x + char_details.size.x, tex_coords.y};
            text_vertices.at(v4).texture_coordinates = {tex_coords.x + char_details.size.x, tex_coords.y + char_details.size.y};

            // for each vertex, assign color, texture, and normalize texture coordinates.
            std::for_each(text_vertices.end() - 4, text_vertices.end(), [&text, texture_unit_index] (auto& vertex)
            {
                vertex.color = text.color;
                vertex.texture_unit_index = texture_unit_index;
                vertex.texture_coordinates = utils::normalize_texture_coords(vertex.texture_coordinates, text.font_atlas->get_texture_size());
            });

            text_index_count += 6;
            x_offset += char_details.advance;
        }
    }
}


static glm::vec2 get_text_dimensions(const Text& text)
{
    // Text dimensions
    // - width: The sum of advances of all characters
    // - height: The max height of a character above the baseline
    uint32_t total_width = 0;
    uint32_t max_height = 0;

    for (char c: text.string)
    {
        const FontAtlas::Character& character_details = text.font_atlas->get_character(c);

        total_width += character_details.advance;
        max_height = glm::max(max_height, static_cast<uint32_t>(character_details.bearing.y));
    }

    return
    {
        static_cast<float>(total_width),
        static_cast<float>(max_height)
    };
}

static glm::vec2 calculate_text_position(const Text& text)
{
    glm::vec2 pos = text.position;
    glm::vec2 text_dimensions = get_text_dimensions(text);

    switch (text.origin)
    {
        case TextOrigin::BOTTOM_LEFT:
            break;
        case TextOrigin::BOTTOM_CENTER:
            pos.x -= text_dimensions.x / 2.f;
            break;
        case TextOrigin::BOTTOM_RIGHT:
            pos.x -= text_dimensions.x;
            break;
        case TextOrigin::TOP_LEFT:
            pos.y -= text_dimensions.y;
            break;
        case TextOrigin::TOP_CENTER:
            pos.x -= text_dimensions.x / 2.f;
            pos.y -= text_dimensions.y;
            break;
        case TextOrigin::TOP_RIGHT:
            pos.x -= text_dimensions.x;
            pos.y -= text_dimensions.y;
            break;
        case TextOrigin::CENTER_LEFT:
            pos.y -= text_dimensions.y / 2.f;
            break;
        case TextOrigin::CENTER:
            pos.x -= text_dimensions.x / 2.f;
            pos.y -= text_dimensions.y / 2.f;
            break;
        case TextOrigin::CENTER_RIGHT:
            pos.x -= text_dimensions.x;
            pos.y -= text_dimensions.y / 2.f;
            break;
        default:
            assert(false);
    }

    return pos;
}
