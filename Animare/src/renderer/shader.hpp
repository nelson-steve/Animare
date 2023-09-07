#pragma once

#include <string>

std::string read_file(const std::string& path) {
    return std::string();
}

class shader {
public:
    shader(const std::string shader_path);

    void bind();
    void unbind();
private:
    uint32_t m_handle;
};