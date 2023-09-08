#pragma once

#include "shader.hpp"

struct GLFWwindow;

class renderer {
public:
    renderer();
    void render();
    void destroy();
private:
    GLFWwindow* m_window;
    uint32_t m_vao, m_vbo;
    shader* m_shader;
};