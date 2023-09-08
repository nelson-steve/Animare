#include "shader.hpp"

#include <iostream>

#include <glad/glad.h>

shader::shader(const std::string vertex_path, const std::string pixel_path)
{
    {
        const std::string& source = read_file(vertex_path);
        compile_vertex_shader(source);
    }
    {
        const std::string& source = read_file(pixel_path);
        compile_pixel_shader(source);
    }
    // linking shaders
    m_program_handle = glCreateProgram();
    glAttachShader(m_program_handle, m_vertex_handle);
    glAttachShader(m_program_handle, m_pixel_handle);
    glLinkProgram(m_program_handle);
    int success = 0;
    glGetProgramiv(m_program_handle, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_program_handle, 512, NULL, infoLog);
        std::cout << "ERROR: Shader linking failed.\n";
        std::cout << "\t" << infoLog << "\n";
        glDeleteShader(m_vertex_handle);
        glDeleteShader(m_pixel_handle);
    }
}

void shader::compile_vertex_shader(const std::string vertex)
{
    m_vertex_handle = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_source = vertex.c_str();
    glShaderSource(m_vertex_handle, 1, &vertex_source, NULL);
    glCompileShader(m_vertex_handle);
    int success = 0;
    glGetShaderiv(m_vertex_handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(m_vertex_handle, 512, NULL, infoLog);
        std::cout << "Vertex compilation failed.\n";
        std::cout << "\t" << infoLog << "\n";
        glDeleteShader(m_vertex_handle);
        return;
    };
}

void shader::compile_pixel_shader(const std::string pixel)
{
    m_pixel_handle = glCreateShader(GL_FRAGMENT_SHADER);
    const char* m_pixel_source = pixel.c_str();
    glShaderSource(m_pixel_handle, 1, &m_pixel_source, NULL);
    glCompileShader(m_pixel_handle);
    int success = 0;
    glGetShaderiv(m_pixel_handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(m_pixel_handle, 512, NULL, infoLog);
        std::cout << "Fragment compilation failed.\n";
        std::cout << "\t" << infoLog << "\n";
        glDeleteShader(m_pixel_handle);
        return;
    };
}

void shader::bind()
{
    glUseProgram(m_program_handle);
}

void shader::unbind()
{
    glUseProgram(0);
}

std::string shader::read_file(const std::string& path) {
    std::string result;
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    }
    else
    {
        assert(false);
    }
    return result;
}