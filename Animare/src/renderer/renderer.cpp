#include "renderer.hpp"

#include "animation/bezier.hpp"
#include "renderer/model.hpp"

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

glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
bezier<glm::vec3> curve;

model* m_model;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

renderer::renderer() {
    // glfw initialization
    if (!glfwInit())    
        assert(false);
    // window creation
    m_window = glfwCreateWindow(1280.0f, 720.0f, "Animare", NULL, NULL);
    if (!m_window)
        assert(false);

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    // loading glfw functions with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        assert(false);

    m_camera = new camera();

    std::string model_path = "../assets/models/DamagedHelmet/glTF/DamagedHelmet.gltf";
    //std::string model_path = "../assets/models/Cube/Cube.gltf";
    m_model = new model();
    m_model->load(model_path);
    
    // creating shaders
    m_shader = new shader("../assets/shaders/vertex.glsl", "../assets/shaders/pixel.glsl");
    m_shader->bind();

    //m_mesh->drawModel();

}

void draw_node(node* _node)
{
    glBindVertexArray(m_model->vao);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->indices_vbo);
    //glBindBuffer(GL_ARRAY_BUFFER, m_model->vertices_vbo);
    if (_node->_mesh) {
        for (primitive* _primitive : _node->_mesh->primitives) {
            uint32_t end = _primitive->first_index - _primitive->index_count;
            //glDrawArrays(GL_ARRAY_BUFFER, 0, _primitive->vertex_count);
            //glDrawRangeElements(GL_TRIANGLES, _primitive->first_index, _primitive->index_count, _primitive->index_count, GL_UNSIGNED_BYTE, 0);
            //vkCmdDrawIndexed(commandBuffer, _primitive->index_count, 1, _primitive->first_index, 0, 0);
            glDrawElements(GL_TRIANGLES, _primitive->index_count, GL_UNSIGNED_INT, 0);
        }
    }
    for (auto& child : _node->children) {
        draw_node(child);
    }
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

        m_camera->Update(delta_time, m_window);

        {
            glm::mat4 model = glm::mat4(1.0f);
            m_shader->bind();
            m_shader->set_uniform_mat4("u_model", model); 
            m_shader->set_uniform_mat4("u_view", m_camera->GetView());
            m_shader->set_uniform_mat4("u_projection", m_camera->GetProjection());
        }

        for (auto& _node : m_model->nodes) {
            draw_node(_node);
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void renderer::destroy() {
    delete m_shader;
    delete m_model;
    delete m_camera;
    glfwTerminate();
}

line::line(const glm::vec3& p1, const glm::vec3& p2) {
    std::vector<glm::vec3> points = {
        p1,
        p2
    };
    m_points = points;
    m_shader = new shader("../assets/shaders/vertex.glsl", "../assets/shaders/pixel.glsl");
    m_shader->bind();

    //glGenVertexArrays(1, &m_vao);
    //glGenBuffers(1, &m_vbo);
    //glBindVertexArray(m_vao);
    //
    //glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(points), points.data(), GL_STATIC_DRAW);
    //
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
}

line::line(const std::vector<glm::vec3>& points) {
    m_points = points;
    m_vertex_count = points.size();
    m_shader = new shader("../assets/shaders/vertex.glsl", "../assets/shaders/pixel.glsl");
    m_shader->bind();
}

void line::draw(camera* cam, const glm::vec3& color) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_shader->bind();
    m_shader->set_uniform_vec3("u_color", color);
    m_shader->set_uniform_mat4("u_model", model);
    m_shader->set_uniform_mat4("u_view", cam->GetView());
    m_shader->set_uniform_mat4("u_projection", cam->GetProjection());
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glDrawArrays(GL_LINES, 0, 6);

    glDeleteBuffers(1, &m_vbo);
}