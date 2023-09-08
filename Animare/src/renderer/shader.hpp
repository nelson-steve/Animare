#pragma once

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
private:
    uint32_t m_program_handle = 0;
    uint32_t m_vertex_handle = 0;
    uint32_t m_pixel_handle = 0;
};