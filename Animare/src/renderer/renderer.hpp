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

class line {
public:
    line(const glm::vec3& point1, const glm::vec3& point2);
    line(const std::vector<glm::vec3>& points);
    void draw(camera* cam, const glm::vec3& color);
private:
    std::vector<glm::vec3> m_points;
    uint32_t m_vertex_count;
    shader* m_shader;
    unsigned int m_vbo, m_vao;
};