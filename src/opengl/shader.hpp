//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_SHADER_HPP
#define PROJECTILEMOTIONSIM_SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    Shader();
    Shader(const std::string& vs_file, const std::string& fs_file);
    ~Shader();

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void bind() const;
    void unbind() const;

    void set_float(const std::string& name, float v0);
    void set_float2(const std::string& name, float v0 , float v1);
    void set_float2(const std::string& name, const glm::vec2& vec2);
    void set_float3(const std::string& name, float v0, float v1, float v2);
    void set_float3(const std::string& name, const glm::vec3& vec3);
    void set_float4(const std::string& name, float v0, float v1, float v2, float v3);
    void set_float4(const std::string& name, const glm::vec4& vec4);
    void set_mat4(const std::string& name, const glm::mat4& matrix);

    static std::string parse_shader(const std::string& file_name);

private:
    uint32_t compile_shader(uint32_t type, const char* source);
    int get_uniform_location(const std::string& name);
    void delete_shader();

private:
    uint32_t m_renderer_id;
    std::unordered_map<std::string, int> m_uniform_location_cache;
};


#endif //PROJECTILEMOTIONSIM_SHADER_HPP
