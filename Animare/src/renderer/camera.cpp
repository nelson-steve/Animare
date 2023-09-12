#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

real xpos = 0.0f;
real ypos = 0.0f;
bool mouse_pos_changed = false;

void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in) {
    xpos = xpos_in;
    ypos = ypos_in;
    mouse_pos_changed = true;
}

camera::camera(GLFWwindow* window, real fov, real near_clip, real far_clip)
    :m_fov(fov),
    m_near_clip(near_clip),
    m_far_clip(far_clip),
    m_projection(glm::perspective(glm::radians(fov), (float)m_viewport_width / m_viewport_height, near_clip, far_clip))
{
    glfwSetCursorPosCallback(window, mouse_callback);
    m_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void camera::update(GLFWwindow* window, real ts)
{
    process_input(window, ts);
    if (mouse_pos_changed && (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)) {
        if (first_mouse)
        {
            lastX = xpos;
            lastY = ypos;
            first_mouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);

        mouse_pos_changed = false;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_world_up));
    m_up = glm::normalize(glm::cross(m_right, m_front));

    glm::perspective(glm::radians(m_fov), (real)m_viewport_width / (real)m_viewport_height, m_near_clip, m_far_clip);
    m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
}

void camera::process_input(GLFWwindow* window, real delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    real cameraSpeed = static_cast<real>(2.5 * delta_time);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_pos += cameraSpeed * m_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_pos -= cameraSpeed * m_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_pos -= glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_pos += glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        m_pos += m_up * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        m_pos -= m_up * cameraSpeed;
}