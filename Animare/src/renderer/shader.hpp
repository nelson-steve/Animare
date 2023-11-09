#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>

class shader {
public:
    shader(const std::string vertex_path, const std::string pixel_path);
    
    static std::string read_file(const std::string& path);
    void compile_vertex_shader(const std::string vertex);
    void compile_pixel_shader(const std::string pixel);
    void bind();
    void unbind();

    void set_uniform_mat4(const std::string& name, const glm::mat4& matrix)
    {
        GLuint location = glGetUniformLocation(m_program_handle, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void set_uniform_vec3(const std::string& name, const glm::vec3& vec)
    {
        GLuint location = glGetUniformLocation(m_program_handle, name.c_str());
        glUniform3f(location, vec[0], vec[1], vec[2]);
    }

    void set_uniform_float(const std::string& name, float value)
    {
        GLuint location = glGetUniformLocation(m_program_handle, name.c_str());
        glUniform1f(location, value);
    }
private:
    uint32_t m_program_handle = 0;
    uint32_t m_vertex_handle = 0;
    uint32_t m_pixel_handle = 0;
};