//
// Created by Gianni on 2/03/2024.
//

#include "renderer2D.hpp"


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


static constexpr uint32_t MAX_QUADS = 20000;
static constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
static constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;
static int MAX_TEXTURES;

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
static std::unique_ptr<IndexBufferStatic> quads_ibo;
static std::vector<uint32_t> quad_indices;
static std::vector<uint32_t> textures;
static const glm::mat4* view_projection_matrix = nullptr;

static glm::vec2 get_text_dimensions(const Text& text);
static glm::vec2 calculate_text_position(const Text& text);



namespace renderer2D
{
    void init_renderer()
    {
        static std::once_flag flag;

        std::call_once(flag, [] ()
        {
            point_vertices.reserve(MAX_VERTICES);
            line_vertices.reserve(MAX_VERTICES);
            text_vertices.reserve(MAX_VERTICES);
            quad_indices.reserve(MAX_INDICES);

            // setup quads indexes
            for (uint32_t i = 0, offset = 0; i < MAX_INDICES; i += 6, offset += 4)
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
            points_vbo = std::make_unique<VertexBufferDynamic>(MAX_VERTICES * sizeof(PointVertex));

            VertexBufferLayout point_line_vertex_layout
            {
                {0, 2, GL_FLOAT, GL_FALSE}, // position
                {1, 4, GL_FLOAT, GL_FALSE}  // color
            };

            points_vao->attach_vertex_buffer(*points_vbo, point_line_vertex_layout);

            // lines setup
            lines_vao = std::make_unique<VertexArray>();
            lines_vbo = std::make_unique<VertexBufferDynamic>(MAX_VERTICES * sizeof(PointVertex));
            lines_vao->attach_vertex_buffer(*lines_vbo, point_line_vertex_layout);

            // text setup
            text_shader = std::make_unique<Shader>("../shaders/text.vert", "../shaders/text.frag");
            text_vao = std::make_unique<VertexArray>();
            text_vbo = std::make_unique<VertexBufferDynamic>(MAX_VERTICES * sizeof(TextVertex));

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
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURES);
            textures.reserve(MAX_TEXTURES);
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
            point_line_shader->set_mat4("u_view_proj", *view_projection_matrix);
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
            point_line_shader->set_mat4("u_view_proj", *view_projection_matrix);
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
            text_shader->set_mat4("u_view_projection", *view_projection_matrix);
            text_vao->bind();

            glDrawElements(GL_TRIANGLES, text_index_count, GL_UNSIGNED_INT, nullptr);

            unbind_textures();
            text_vao->unbind();
            text_shader->unbind();
        }
    }

    void pre_render(const OrthographicCamera& camera)
    {
        view_projection_matrix = &camera.get_view_projection_matrix();
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
        if (point_vertices.size() == MAX_VERTICES)
            render_points();

        point_vertices.push_back({{x, y}, color});
    }

    void draw_line(float x1, float y1, float x2, float y2, const glm::vec4& color)
    {
        if (line_vertices.size() + 2 > MAX_VERTICES)
            render_lines();

        line_vertices.push_back({{x1, y1}, color});
        line_vertices.push_back({{x2, y2}, color});
    }

    void draw_text(const Text& text)
    {
        assert(text.font_atlas);

        // flush current batch if the max number of vertices will be surpassed in this function
        if (text_vertices.size() + text.string.size() * 4 >= MAX_VERTICES)
            render();

        // check if font texture is in textures
        size_t texture_unit_index;
        auto itr = std::find(textures.begin(), textures.end(), text.font_atlas->get_texture_id());
        if (itr == textures.end())
        {
            // the number of textures is at max textures, so flush current batch
            if (textures.size() == MAX_TEXTURES)
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
        for (const auto c : text.string)
        {
            const FontAtlas::Character& character_details = text.font_atlas->get_character(c);

            // don't create vertices for space. Just advance the offset
            if (c == ' ')
            {
                x_offset += character_details.advance;
                continue;
            }

            glm::vec2 position
            {
                text_origin.x + x_offset + character_details.bearing.x,
                text_origin.y - (character_details.size.y - character_details.bearing.y)
            };

            const glm::uvec2& texture_coordinates = character_details.tex_coords;

            // the index of each vertex inside the vertices vector
            const size_t v1 = text_vertices.size();
            const size_t v2 = v1 + 1;
            const size_t v3 = v1 + 2;
            const size_t v4 = v1 + 3;

            // insert 4 vertices (quad) into the vector
            text_vertices.insert(text_vertices.end(), 4, TextVertex());

            // positions of quad vertices
            text_vertices.at(v1).position = position; // top left
            text_vertices.at(v2).position = {position.x + character_details.size.x, position.y}; // top right
            text_vertices.at(v3).position = {position.x + character_details.size.x, position.y + character_details.size.y}; // bottom right
            text_vertices.at(v4).position = {position.x, position.y + character_details.size.y}; // bottom left

            // texture coordinates of quad vertices
            text_vertices.at(v1).texture_coordinates = {texture_coordinates.x, texture_coordinates.y + character_details.size.y};
            text_vertices.at(v2).texture_coordinates = texture_coordinates + character_details.size;
            text_vertices.at(v3).texture_coordinates = {texture_coordinates.x + character_details.size.x, texture_coordinates.y};
            text_vertices.at(v4).texture_coordinates = texture_coordinates;

            // for each vertex, assign color, texture, and normalize texture coordinates.
            std::for_each(text_vertices.end() - 4, text_vertices.end(), [&text, texture_unit_index] (auto& vertex)
            {
                vertex.color = text.color;
                vertex.texture_unit_index = texture_unit_index;
                vertex.texture_coordinates = utils::normalize_texture_coords(vertex.texture_coordinates, text.font_atlas->get_texture_size());
            });

            text_index_count += 6;
            x_offset += character_details.advance;
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
