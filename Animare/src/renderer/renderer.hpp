#pragma once

#include "mesh.hpp"
#include "shader.hpp"
#include "defines.hpp"
#include "camera.hpp"

struct GLFWwindow;

class renderer {
public:
    renderer();
    void render();
    void destroy();
private:
    GLFWwindow* m_window;
    camera* m_camera;
    mesh* m_mesh;
    uint32_t m_vao, m_vbo;
    shader* m_shader;
    real delta_time, last_frame;
};