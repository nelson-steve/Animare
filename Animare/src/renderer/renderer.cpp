#include "renderer.hpp"

#include "bezier.hpp"

#include <assert.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "tiny_gltf.h"

float vertices[] = {
     -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f,  0.5f, 0.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
std::vector<glm::vec3> points;
renderer::renderer() {
    // glfw initialization
    if (!glfwInit())    
        assert(false);
    // window creation
    m_window = glfwCreateWindow(1280, 720, "Animare", NULL, NULL);
    if (!m_window)
        assert(false);

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    // loading glfw functions with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        assert(false);

    m_camera = new camera(m_window);

    // creating shaders
    m_shader = new shader("../assets/shaders/vertex.glsl", "../assets/shaders/pixel.glsl");
    m_shader->bind();

    std::string model_path = "../assets/models/DamagedHelmet/glTF/DamagedHelmet.gltf";
    m_mesh = new mesh(model_path);
    
    m_mesh->bindModel();
    bezier<glm::vec3> curve;
    curve.point1 = glm::vec3(-0.5, 0, 0);
    curve.point2 = glm::vec3(0.5, 0, 0);
    curve.control1 = glm::vec3(-0.2, 0.1, 0);
    curve.control2 = glm::vec3(0.2, 0.1, 0);
    points.push_back(curve.point1);
    points.push_back(curve.point2);
    points.push_back(curve.control1);
    points.push_back(curve.control2);
}

void renderer::render() {

    while (!glfwWindowShouldClose(m_window))
    {
        real current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_camera->update(m_window, delta_time);

        {
            glm::mat4 model = glm::mat4(1.0f);
            m_shader->bind();
            m_shader->set_uniform_mat4("u_model", model);
            m_shader->set_uniform_mat4("u_view", m_camera->get_view_matrix());
            m_shader->set_uniform_mat4("u_projection", m_camera->get_projection_matrix());
        }

        m_mesh->drawModel();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void renderer::destroy() {
    delete m_shader;
    delete m_mesh;
    delete m_camera;
    glfwTerminate();
}