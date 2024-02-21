//
// Created by Gianni on 19/02/2024.
//

#include "shader.hpp"


Shader::Shader()
    : m_renderer_id()
{
}

Shader::Shader(const std::string &vs_file, const std::string &fs_file)
{
    std::string vs_src = parse_shader(vs_file);
    std::string fs_src = parse_shader(fs_file);

    uint32_t vs = compile_shader(GL_VERTEX_SHADER, vs_src.c_str());
    uint32_t fs = compile_shader(GL_FRAGMENT_SHADER, fs_src.c_str());

    m_renderer_id = glCreateProgram();

    glAttachShader(m_renderer_id, vs);
    glAttachShader(m_renderer_id, fs);
    glLinkProgram(m_renderer_id);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()
{
    delete_shader();
}

Shader::Shader(Shader &&other) noexcept
{
    m_renderer_id = other.m_renderer_id;
    m_uniform_location_cache = std::move(other.m_uniform_location_cache);

    other.delete_shader();
}

Shader &Shader::operator=(Shader &&other) noexcept
{
    if (this != &other)
    {
        if (m_renderer_id)
        {
            delete_shader();
        }

        m_renderer_id = other.m_renderer_id;
        m_uniform_location_cache = std::move(other.m_uniform_location_cache);

        other.delete_shader();
    }

    return *this;
}

void Shader::bind() const
{
    glUseProgram(m_renderer_id);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::set_float(const std::string &name, float v0)
{
    glUniform1f(get_uniform_location(name), v0);
}

void Shader::set_float2(const std::string &name, float v0, float v1)
{
    glUniform2f(get_uniform_location(name), v0, v1);
}

void Shader::set_float2(const std::string &name, const glm::vec2 &vec2)
{
    glUniform2f(get_uniform_location(name), vec2.x, vec2.y);
}

void Shader::set_float3(const std::string &name, float v0, float v1, float v2)
{
    glUniform3f(get_uniform_location(name), v0, v1, v2);
}

void Shader::set_float3(const std::string &name, const glm::vec3 &vec3)
{
    glUniform3f(get_uniform_location(name), vec3.x, vec3.y, vec3.z);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &matrix)
{
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::parse_shader(const std::string &file_name)
{
    std::ifstream file(file_name);

    if (!file.is_open())
    {
        throw std::runtime_error("Shader::parse_shader: Failed to open file " + file_name);
    }

    std::ostringstream oss;
    oss << file.rdbuf();

    return oss.str();
}

uint32_t Shader::compile_shader(uint32_t type, const char *source)
{
    uint32_t shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &source, nullptr);
    glCompileShader(shader_id);
    return shader_id;
}

int Shader::get_uniform_location(const std::string &name)
{
    if (auto result = m_uniform_location_cache.find(name);
        result != m_uniform_location_cache.end())
    {
        return result->second;
    }

    int location = glGetUniformLocation(m_renderer_id, name.c_str());

    if (location == -1)
    {
        std::cout << "Warning: Uniform " + name + " doesn't exist or it may not be used\n";
    }

    m_uniform_location_cache[name] = location;

    return location;
}

void Shader::delete_shader()
{
    if (m_renderer_id)
    {
        glDeleteProgram(m_renderer_id);
        m_renderer_id = 0;

        m_uniform_location_cache.clear();
    }
}
