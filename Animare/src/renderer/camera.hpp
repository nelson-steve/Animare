#pragma once

#include "defines.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class camera {
public:
    camera(GLFWwindow* window, real fov = 45.0f, real near_clip = 0.1f, real far_clip = 1000.0f);

    void update(GLFWwindow* window, real ts);
    void process_input(GLFWwindow* window, real delta_time);

    glm::mat4 get_view_matrix() const { return m_view; }
    glm::mat4 get_projection_matrix() const { return m_projection; }
private:
    bool first_mouse = true;
    real m_fov = 45.0f, m_near_clip = 0.1f, m_far_clip = 1000.0f;
    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_projection = glm::mat4(1.0f);
    real m_viewport_width = 1280, m_viewport_height = 720;
    glm::vec3 m_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right = glm::vec3(1.0f, 0.0, 0.0);
    glm::vec3 m_world_up = glm::vec3(1.0f);
    real m_yaw = -90.0f;
    real m_pitch = 0.0f;
    real m_movementSpeed = 2.5f;
    real m_mouse_sensitivity = 0.1f;
    real m_zoom = 45.0f;
    real lastX = 800.0f / 2.0;
    real lastY = 600.0 / 2.0;
};